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

	return true;
}

bool SlimeEnemy::PreUpdate() {

	return true;
}

bool SlimeEnemy::Update()
{
	currentAnim = &idleEnemy;

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x - (width / 4));
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y - (height / 3));

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