#include "SlimeEnemy.h"
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


SlimeEnemy::SlimeEnemy() : Entity(EntityType::ENEMY)
{
	name.Create("Slime");
}

SlimeEnemy::~SlimeEnemy() {

}

bool SlimeEnemy::Awake() {

	return true;
}

bool SlimeEnemy::Start() {

	//L02: DONE 1: Initialize Player parameters
	startPos.x = parameters.attribute("x").as_int();
	startPos.y = parameters.attribute("y").as_int();

	origin.x = startPos.x;
	origin.y = startPos.y;

	texturePath = parameters.attribute("texturepath").as_string();


	width = 32;
	height = 32;

	idleEnemy.PushBack({ 21, 30, 20, 19 });
	idleEnemy.PushBack({ 86, 30, 18, 19 });
	idleEnemy.PushBack({ 148, 30, 22, 19 });
	idleEnemy.PushBack({ 211, 30, 24, 19 });
	idleEnemy.loop = true;
	idleEnemy.speed = 0.1f;

	runEnemy.PushBack({ 24, 42, 16, 23 });
	runEnemy.PushBack({ 88, 41, 15, 21 });
	runEnemy.PushBack({ 152, 39, 15, 21 });
	runEnemy.PushBack({ 216, 40, 15, 22 });
	runEnemy.PushBack({ 280, 42, 16, 23 });
	runEnemy.PushBack({ 344, 41, 18, 21 });
	runEnemy.PushBack({ 408, 39, 18, 22 });
	runEnemy.PushBack({ 472, 40, 18, 22 });
	runEnemy.loop = true;
	runEnemy.speed = 0.1f;
	
	jumpEnemy.PushBack({ 146, 102, 28, 11 });
	jumpEnemy.PushBack({ 210, 100, 26, 13 });
	jumpEnemy.PushBack({ 280, 89, 14, 24 });
	jumpEnemy.PushBack({ 342, 87, 19, 14 });
	jumpEnemy.PushBack({ 23, 149, 19, 14 });
	jumpEnemy.PushBack({ 88, 150, 14, 22 });
	jumpEnemy.PushBack({ 210, 166, 27, 11 });
	jumpEnemy.PushBack({ 278, 158, 20, 19 });
	jumpEnemy.loop = false;
	jumpEnemy.speed = 0.09f;

	dieEnemy.PushBack({ 142, 218, 31, 22 });
	dieEnemy.PushBack({ 206, 218, 31, 22 });
	dieEnemy.PushBack({ 271, 222, 31, 22 });
	dieEnemy.PushBack({ 16, 282, 31, 22 });
	dieEnemy.PushBack({ 81, 282, 31, 22 });
	dieEnemy.PushBack({ 145, 282, 31, 22 });
	dieEnemy.loop = false;
	dieEnemy.speed = 0.1f;

	hitEnemy.PushBack({213, 286, 18, 19 });
	hitEnemy.loop = false;
	hitEnemy.speed = 0.1f;

	//initilize textures
	texture = app->tex->Load(texturePath);

	//Initialize SFX
	stompSFXPath = app->configNode.child("scene").child("slimesfx").attribute("stompSFXPath").as_string();
	powerUpSFXPath = app->configNode.child("scene").child("slimesfx").attribute("powerUpSFXPath").as_string();
	slimeHitSFXPath = app->configNode.child("scene").child("slimesfx").attribute("slimeHitSFXPath").as_string();

	// Loading the set of SFX, BETTER HERE FOR ENABLE/DISABLE
	stompSFX = app->audio->LoadFx(stompSFXPath);
	powerUpSFX = app->audio->LoadFx(powerUpSFXPath);
	slimeHitSFX = app->audio->LoadFx(slimeHitSFXPath);

	currentAnim = &idleEnemy;
	dead = false;
	lives = 2;

	//Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircle(startPos.x, startPos.y, width / 4.5, bodyType::DYNAMIC, ColliderType::ENEMY);

	pbody->listener = this;

	hitbox = app->physics->CreateRectangle(METERS_TO_PIXELS(pbody->body->GetTransform().p.x), METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 13, 13, 4, bodyType::DYNAMIC, ColliderType::SLIME_HITBOX);

	hitbox->listener = this;

	refreshPathTime = 0;

	jump = false;
	ableAttack = true;

	return true;
}

bool SlimeEnemy::PreUpdate() {

	return true;
}

bool SlimeEnemy::Update()
{
	currentAnim = &idleEnemy;
	velocity.y = -GRAVITY_Y;

	if (app->scene->gamePaused != true)
	{
		// Being hit anim if player attacks the slime
		if (onCollision) {
			currentAnim = &hitEnemy;

			if (hitEnemy.HasFinished()) {
				onCollision = false;
				hitEnemy.Reset();
			}
		}

		//Takes player pos for the path destination
		iPoint playerTile = app->map->WorldToMap(app->scene->player->position.x + 32, app->scene->player->position.y);

		//Calculates distance between slime and player for detection range
		float distance = playerTile.x - origin.x;

		//Test compute path function
		if (originSelected == true && distance <= 10 && distance >= -10)
		{
			app->pathfinding->CreatePath(origin, playerTile);
			refreshPathTime++;
			originSelected = false;

			MovementDirection(origin, playerTile);
			Attack(origin, playerTile);
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

		if (jump == false)
			pbody->body->SetLinearVelocity(velocity);
		else if (jump == true)
			currentAnim = &jumpEnemy;

		if (ableAttack == false) {
			attackCooldown++;
			if (attackCooldown >= 50)
				ableAttack = true;
		}


		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x - (width / 4));
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y - (height / 3));
	}

	//hitbox->body->SetGravityScale(0);
	hitboxPos.x = pbody->body->GetTransform().p.x;
	hitboxPos.y = pbody->body->GetTransform().p.y - PIXEL_TO_METERS(10);
	hitbox->body->SetTransform({ hitboxPos.x, hitboxPos.y }, 0);

	if (app->scene->gamePaused == true)
		pbody->body->SetLinearVelocity({ 0,0 });


	if(dead == true)
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

bool SlimeEnemy::PostUpdate() {


	return true;
}

bool SlimeEnemy::CleanUp()
{

	return true;
}

void SlimeEnemy::MovementDirection(const iPoint& origin, const iPoint& destination) {
	
	float resX = destination.x - origin.x;
	float resY = destination.y - origin.y;

	if (app->pathfinding->IsWalkable(destination) != 0) {

		//Check if player is to the right or the left of the origin
		if (resX < 0 || app->scene->player->position.x + 32 < position.x) {
			velocity.x = -3;
			fliped = SDL_FLIP_NONE;
		}
		if (resX > 0 || app->scene->player->position.x > position.x) {
			velocity.x = +3;
			fliped = SDL_FLIP_HORIZONTAL;
		}

		/*ENEMY JUMP FOR 2 TILES HEIGHT, 15 iS The Lowest Platform Tile Height*/
		if (resY < 0 && jump == false && app->pathfinding->GetNextTileY(2) < 15 && app->pathfinding->GetNextTileY(2) > 13) {
			jumpEnemy.Reset();
			jump = true;
			pbody->body->ApplyLinearImpulse({ 0, -1.2 }, pbody->body->GetWorldCenter(), true);
		}
		/*ENEMY JUMP FOR 4 TILES HEIGHT, 15 iS The Lowest Platform Tile Height*/
		if (resY < 0 && jump == false && app->pathfinding->GetNextTileY(2) < 14 && app->pathfinding->GetNextTileY(2) > 12) {
			jumpEnemy.Reset();
			jump = true;
			pbody->body->ApplyLinearImpulse({ 0, -1.5 }, pbody->body->GetWorldCenter(), true);
		}
		
	}
	else {
		velocity.x = 0;
	}
	
		
}

void SlimeEnemy::Attack(const iPoint& origin, const iPoint& destination) {
	
	float resX = destination.x - origin.x;

	/*Slime Attack Left*/
	if (resX < -2 && resX > -7 && jump == false && ableAttack == true) {
		ableAttack = false;
		attackCooldown = 0;
		pbody->body->ApplyLinearImpulse({ -25, 0 }, pbody->body->GetLocalCenter(), true); //Hace un dash hacia el player-->En este caso a la izquierda
	}
		

	/*Slime Attack Right*/
	if (resX > 2 && resX < 7 && jump == false && ableAttack == true) {
		ableAttack = false;
		attackCooldown = 0;
		pbody->body->ApplyLinearImpulse({ 25, 0 }, pbody->body->GetLocalCenter(), true); //Hace un dash hacia el player-->En este caso a la derecha
	}
		
		
}

void SlimeEnemy::OnCollision(PhysBody* physA, PhysBody* physB) {

	// L07 DONE 7: Detect the type of collision
	if (physA->cType == ColliderType::ENEMY) {
		switch (physB->cType)
		{
		case ColliderType::PLATFORM:
			LOG("Collision PLATFORM");
			if (jump == true)
				jump = false;
			break;
		case ColliderType::WATER:
			LOG("Collision WATER");
			dead = true;
			break;
		case ColliderType::ENEMY:
			LOG("Collision ENEMY");
			break;
		case ColliderType::PLAYER_ATTACK_HITBOX:
			LOG("Collision PLAYER ATTACK HITBOX");
			lives--;
			if (lives <= 0) {
				dead = true;
			}
			onCollision = true;
			app->audio->PlayFx(slimeHitSFX);
			break;
		case ColliderType::WALL:
			LOG("Collision WALL");
			break;
		case ColliderType::UNKNOWN:
			LOG("Collision UNKNOWN");
			break;
		}
	}
	
	if (physA->cType == ColliderType::SLIME_HITBOX) {
		switch (physB->cType)
		{
		case ColliderType::PLAYER_ATTACK_HITBOX:
			LOG("Collision PLAYER ATTACK HITBOX");
			lives--;
			if (lives <= 0) {
				dead = true;
			}
			onCollision = true;
			app->audio->PlayFx(slimeHitSFX);
			break;
		case ColliderType::PLAYER:
			LOG("Collision WALL");
			lives--;
			if (lives <= 0) {
				dead = true;
			}
			onCollision = true;
			app->audio->PlayFx(slimeHitSFX);
			//dead = true;
			break;
		}
	}
	

}

void SlimeEnemy::ResetSlime() {

	pbody->body->SetSleepingAllowed(false);


}