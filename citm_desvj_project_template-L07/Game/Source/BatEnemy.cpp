#include "BatEnemy.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Map.h"
#include "PathFinding.h"
#include "ModuleFadeToBlack.h"
#include "EntityManager.h"


BatEnemy::BatEnemy() : Entity(EntityType::FLYING_ENEMY)
{
	name.Create("Bat");
}

BatEnemy::~BatEnemy() {

}

bool BatEnemy::Awake() {

	return true;
}

bool BatEnemy::Start() {

	//L02: DONE 1: Initialize Player parameters
	startPos.x = parameters.attribute("x").as_int();
	startPos.y = parameters.attribute("y").as_int();

	origin.x = startPos.x;
	origin.y = startPos.y;

	texturePath = parameters.attribute("texturepath").as_string();


	width = 32;
	height = 32;

	flyingEnemy.PushBack({ 87, 24, 18, 20});
	flyingEnemy.PushBack({ 151, 24, 18, 20});
	flyingEnemy.PushBack({ 215, 24, 18, 20 });
	flyingEnemy.PushBack({ 279, 24, 18, 20 });
	flyingEnemy.loop = true;
	flyingEnemy.speed = 0.1f;

	hitEnemy.PushBack({ 281, 288, 18, 20 });
	hitEnemy.PushBack({ 279, 155, 18, 20 });
	hitEnemy.PushBack({ 23, 216, 18, 20 });
	hitEnemy.PushBack({ 87, 216, 18, 20 });
	hitEnemy.loop = false;
	hitEnemy.speed = 0.1f;

	dieEnemy.PushBack({ 281, 288, 18, 20 });
	dieEnemy.PushBack({ 279, 155, 18, 20 });
	dieEnemy.PushBack({ 23, 216, 18, 20 });
	dieEnemy.PushBack({ 87, 216, 18, 20 });
	dieEnemy.PushBack({ 151, 216, 18, 20 });
	dieEnemy.PushBack({ 216, 216, 18, 20 });
	dieEnemy.PushBack({ 279, 216, 18, 20 });
	dieEnemy.PushBack({ 23, 281, 18, 20 });
	dieEnemy.loop = false;
	dieEnemy.speed = 0.1f;

	//initilize textures
	texture = app->tex->Load(texturePath);

	//Initialize SFX
	stompSFXPath = app->configNode.child("scene").child("slimesfx").attribute("stompSFXPath").as_string();		// same as slime
	powerUpSFXPath = app->configNode.child("scene").child("slimesfx").attribute("powerUpSFXPath").as_string();	// same as slime
	batHitSFXPath = app->configNode.child("scene").child("batsfx").attribute("batHitSFXPath").as_string();

	// Loading the set of SFX, BETTER HERE FOR ENABLE/DISABLE
	stompSFX = app->audio->LoadFx(stompSFXPath);
	powerUpSFX = app->audio->LoadFx(powerUpSFXPath);
	batHitSFX = app->audio->LoadFx(batHitSFXPath);

	currentAnim = &flyingEnemy;
	dead = false;
	lives = 2;

	//Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircle(startPos.x, startPos.y, width / 4.5, bodyType::DYNAMIC, ColliderType::FLYING_ENEMY);

	pbody->listener = this;

	hitbox = app->physics->CreateRectangle(METERS_TO_PIXELS(pbody->body->GetTransform().p.x), METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 15, 13, 4, bodyType::DYNAMIC, ColliderType::BAT_HITBOX);

	refreshPathTime = 0;

	return true;
}

bool BatEnemy::PreUpdate() {

	return true;
}

bool BatEnemy::Update()
{
	
	currentAnim = &flyingEnemy;
	//velocity = { 0, 0 };
	pbody->body->SetGravityScale(0);

	if (app->scene->gamePaused != true)
	{
		// Being hit anim if player attacks the bat
		if (onCollision) {
			currentAnim = &hitEnemy;

			if (hitEnemy.HasFinished()) {
				onCollision = false;
				hitEnemy.Reset();
			}
		}

		//Takes player pos for the path destination
		iPoint playerTile = app->map->WorldToMap(app->scene->player->position.x + 32, app->scene->player->position.y);


		//Calculates distance between bat and player for detection range
		float distance = playerTile.x - origin.x;

		//Test compute path function
		if (originSelected == true && distance <= 15 && distance >= -15)
		{
			app->pathfinding->CreatePath(origin, playerTile);
			refreshPathTime++;
			originSelected = false;
			/*if (refreshPathTime >= 150)
				originSelected = false;*/
			MovementDirection(origin, playerTile);
		}
		else
		{
			velocity = { 0, 0 };
			origin.x = pbody->body->GetPosition().x;
			origin.y = pbody->body->GetPosition().y;
			originSelected = true;
			app->pathfinding->ClearLastPath();
			refreshPathTime = 0;
		}

		pbody->body->SetLinearVelocity(velocity);

		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x - (width / 4));
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y - (height / 3));
	}

	//hitbox->body->SetGravityScale(0);
	hitboxPos.x = pbody->body->GetTransform().p.x;
	hitboxPos.y = pbody->body->GetTransform().p.y - PIXEL_TO_METERS(10);
	hitbox->body->SetTransform({ hitboxPos.x, hitboxPos.y }, 0);

	if(app->scene->gamePaused == true)
		pbody->body->SetLinearVelocity({ 0,0 });

	if (dead == true)
	{
		currentAnim = &dieEnemy;

		//Destroy entity
		app->entityManager->DestroyEntity(this);
		app->physics->world->DestroyBody(pbody->body);
		app->physics->world->DestroyBody(hitbox->body);
		app->audio->PlayFx(powerUpSFX);
		dead = false;
	}

	if (app->scene->gamePaused != true)
	{
		if (app->physics->debug)
		{
			// L12: Get the latest calculated path and draw
			const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
			//LOG("Path Count: %d", path->Count());
			for (uint i = 0; i < path->Count(); ++i)
			{
				iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
				app->render->DrawTexture(app->scene->slimeTilePathTex, pos.x, pos.y);
			}

			// L12: Debug pathfinding
			iPoint originScreen = app->map->MapToWorld(origin.x, origin.y);
			app->render->DrawTexture(app->scene->originTex, originScreen.x - 16, originScreen.y - 19);
		}
	}

	if (app->scene->gamePaused != true)
	{
		SDL_Rect rect = currentAnim->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x, position.y, &rect, fliped);
		currentAnim->Update();
	}

	return true;
}

bool BatEnemy::PostUpdate() {


	return true;
}

bool BatEnemy::CleanUp()
{

	return true;
}

void BatEnemy::MovementDirection(const iPoint& origin, const iPoint& destination) {

	float resX = destination.x - origin.x;
	float resY = destination.y - origin.y;

	iPoint playerTile = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);
	if (app->pathfinding->IsWalkable(playerTile) != 0) {
		//Check if player is to the right or the left of the origin
		if (resX < 0 || app->scene->player->position.x+32 < position.x) {
			velocity.x = -5;
			fliped = SDL_FLIP_HORIZONTAL;
		}
		if (resX > 0 || app->scene->player->position.x > position.x) {
			velocity.x = +5;
			fliped = SDL_FLIP_NONE;
		}
		if (resY < 0 || app->scene->player->position.y+32 < position.y) {
			velocity.y = -5;
		}
		if (resY > 0 || app->scene->player->position.y > position.y) {
			velocity.y = +5;
		}
	}
	else {
		velocity.x = 0;
		velocity.y = 0;
	}
}

void BatEnemy::OnCollision(PhysBody* physA, PhysBody* physB) {

	// L07 DONE 7: Detect the type of collision

	switch (physB->cType)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::WATER:
		LOG("Collision WATER");
		dead = true;
		break;
	case ColliderType::ENEMY:
		LOG("Collision ENEMY");
		break;
	case ColliderType::FLYING_ENEMY:
		LOG("Collision FLYING ENEMY");
		break;
	case ColliderType::PLAYER_ATTACK_HITBOX:
		LOG("Collision PLAYER ATTACK HITBOX");
		lives--;
		if (lives <= 0) {
			dead = true;
		}
		onCollision = true;
		app->audio->PlayFx(batHitSFX);
		break;
	case ColliderType::WALL:
		LOG("Collision WALL");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}

}

void BatEnemy::ResetBat() {

	pbody->body->SetSleepingAllowed(false);


}