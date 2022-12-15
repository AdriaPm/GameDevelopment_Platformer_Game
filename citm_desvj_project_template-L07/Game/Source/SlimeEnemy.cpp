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


SlimeEnemy::SlimeEnemy() : Entity(EntityType::ENEMY)
{
	name.Create("Slime");
}

SlimeEnemy::~SlimeEnemy() {

}

bool SlimeEnemy::Awake() {

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

	dieEnemy.PushBack({ 23, 202, 16, 22 });
	dieEnemy.PushBack({ 87, 206, 21, 18 });
	dieEnemy.PushBack({ 153, 213, 25, 11 });
	dieEnemy.PushBack({ 217, 212, 25, 12 });
	dieEnemy.PushBack({ 281, 213, 25, 11 });
	dieEnemy.loop = false;
	dieEnemy.speed = 0.1f;

	return true;
}

bool SlimeEnemy::Start() {
	//initilize textures
	texture = app->tex->Load(texturePath);

	// Loading the set of SFX, BETTER HERE FOR ENABLE/DISABLE
	/*jumpSFX = app->audio->LoadFx("Assets/Audio/Fx/jump.wav");
	dieSFX = app->audio->LoadFx("Assets/Audio/Fx/death.wav");*/

	currentAnim = &idleEnemy;
	dead = false;

	//Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircle(startPos.x, startPos.y, width / 4.5, bodyType::DYNAMIC, ColliderType::ENEMY);

	pbody->listener = this;

	hitbox = app->physics->CreateRectangleSensor(METERS_TO_PIXELS(pbody->body->GetTransform().p.x), METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 15, 5, 2, bodyType::STATIC, ColliderType::SLIME_HITBOX);

	refreshPathTime = 0;

	return true;
}

bool SlimeEnemy::PreUpdate() {

	return true;
}

bool SlimeEnemy::Update()
{
	currentAnim = &idleEnemy;
	velocity = { 0, -GRAVITY_Y };

	//Takes player pos for the path destination
	iPoint playerTile = app->map->WorldToMap(app->scene->player->position.x + 32, app->scene->player->position.y);

	//Check if the enemy is visible on camera, if not, don't create path and don't move
	if (pbody->body->GetPosition().x > app->render->camera.x - app->render->camera.w/2 && pbody->body->GetPosition().x < app->render->camera.x + app->render->camera.w/2)
	{
		//Test compute path function
		if (originSelected == true)
		{
			app->pathfinding->CreatePath(origin, playerTile);
			refreshPathTime++;
			originSelected = false;
			/*if (refreshPathTime >= 150)
				originSelected = false;*/
		}
		else
		{
			origin.x = pbody->body->GetPosition().x;
			origin.y = pbody->body->GetPosition().y;
			originSelected = true;
			app->pathfinding->ClearLastPath();
			refreshPathTime = 0;
		}

		MovementDirection(origin, playerTile);
	}
	else 
	{
		app->pathfinding->ClearLastPath();
		refreshPathTime = 0;
	}
	
	pbody->body->SetLinearVelocity(velocity);

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x - (width / 4));
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y - (height / 3));

	//hitbox->body->SetGravityScale(0);
	hitboxPos.x = pbody->body->GetTransform().p.x;
	hitboxPos.y = pbody->body->GetTransform().p.y - PIXEL_TO_METERS(10);
	hitbox->body->SetTransform({ hitboxPos.x, hitboxPos.y }, 0);

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

	if (lives == 0) 
	{
		app->scene->slime->dead = true;
		
		//Destroy entity

	}

	SDL_Rect rect = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect, fliped);
	currentAnim->Update();

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
	
	float res = destination.x - origin.x;
	iPoint playerTile = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);
	if (app->pathfinding->IsWalkable(playerTile) != 0) {
		//Check if player is to the right or the left of the origin
		if (res < 0) {
			velocity.x = -2;
			fliped = SDL_FLIP_NONE;
		}
		if (res > 0) {
			velocity.x = +2;
			fliped = SDL_FLIP_HORIZONTAL;
		}
	}
	else {
		velocity.x = 0;
	}
	
		
}

void SlimeEnemy::OnCollision(PhysBody* physA, PhysBody* physB) {

	// L07 DONE 7: Detect the type of collision

	switch (physB->cType)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::WATER:
		LOG("Collision WATER");
		break;
	case ColliderType::ENEMY:
		LOG("Collision ENEMY");
		break;
	case ColliderType::WALL:
		LOG("Collision WALL");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}

}