#include "Player.h"
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


Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	//L02: DONE 1: Initialize Player parameters
	//pos = position;
	//texturePath = "Assets/Textures/player/idle1.png";
	//L02: DONE 5: Get Player parameters from XML
	startPos.x = parameters.attribute("x").as_int();
	startPos.y = parameters.attribute("y").as_int();
	/*position.x = startingPosition.x;
	position.y = startingPosition.y;*/

	texturePath = parameters.attribute("texturepath").as_string();

	width = 32;
	height = 32;

	idlePlayer.PushBack({ 23, 10, 17, 23 });
	idlePlayer.PushBack({ 87, 10, 17, 23 });
	idlePlayer.PushBack({ 151, 10, 17, 23 });
	idlePlayer.PushBack({ 215, 10, 17, 23 });
	idlePlayer.PushBack({ 279, 10, 17, 23 });
	idlePlayer.PushBack({ 343, 10, 17, 23 });
	idlePlayer.PushBack({ 407, 10, 17, 23 });
	idlePlayer.PushBack({ 471, 10, 17, 23 });
	idlePlayer.loop = true;
	idlePlayer.speed = 0.1f;
	
	runPlayer.PushBack({ 24, 42, 16, 23 });
	runPlayer.PushBack({ 88, 41, 15, 21 });
	runPlayer.PushBack({ 152, 39, 15, 21 });
	runPlayer.PushBack({ 216, 40, 15, 22 });
	runPlayer.PushBack({ 280, 42, 16, 23 });
	runPlayer.PushBack({ 344, 41, 18, 21 });
	runPlayer.PushBack({ 408, 39, 18, 22 });
	runPlayer.PushBack({ 472, 40, 18, 22 });
	runPlayer.loop = true;
	runPlayer.speed = 0.1f;

	diePlayer.PushBack({ 23, 202, 16, 22 });
	diePlayer.PushBack({ 87, 206, 21, 18 });
	diePlayer.PushBack({ 153, 213, 25, 11 });
	diePlayer.PushBack({ 217, 212, 25, 12 });
	diePlayer.PushBack({ 281, 213, 25, 11 });
	diePlayer.loop = false;
	diePlayer.speed = 0.1f;

	return true;
}

bool Player::Start() {
	//initilize textures
	texture = app->tex->Load(texturePath);

	// Loading the set of SFX, BETTER HERE FOR ENABLE/DISABLE
	jumpSFX = app->audio->LoadFx("Assets/Audio/Fx/jump.wav");
	dieSFX = app->audio->LoadFx("Assets/Audio/Fx/death.wav");
	pickCoinSFX = app->audio->LoadFx("Assets/Audio/Fx/pick_coin.wav");
	levelCompletedSFX = app->audio->LoadFx("Assets/Audio/Fx/level_completed.wav");
	selectSFX = app->audio->LoadFx("Assets/Audio/Fx/select.wav");

	currentAnim = &idlePlayer;
	dead = false;

	//Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircle(position.x, position.y, width / 3, bodyType::DYNAMIC, ColliderType::PLAYER);

	pbody->listener = this;

	return true;
}

bool Player::PreUpdate() {

	return true;
}

bool Player::Update()
{
	currentAnim = &idlePlayer;

	//Enable/Disable Debug
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		app->physics->debug = !app->physics->debug;
		app->audio->PlayFx(selectSFX);
	}

	if (godMode == true) {

		velocity = { 0, 0 };
		pbody->body->SetGravityScale(0);

		// Fly around the map
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			velocity.y = -5;
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			velocity.y = 5;
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			isFliped = true;
			velocity.x = -5;
			if (isFliped == true && fliped == SDL_FLIP_NONE) {
				fliped = SDL_FLIP_HORIZONTAL;
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			isFliped = false;
			velocity.x = 5;
			if (isFliped == false && fliped == SDL_FLIP_HORIZONTAL) {
				fliped = SDL_FLIP_NONE;
			}
		}


	}
	else if(godMode == false && dead == false)
	{
		velocity = { 0, -GRAVITY_Y };

		//L02: DONE 4: modify the position of the player using arrow keys and render the texture
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {

			longPress = false;

			if (onGround == true) {
				jumping = true;
				jumpingTime = 0;

				app->audio->PlayFx(jumpSFX);
			}
			else if (onGround == false && jumpCount > 1 && jumping == true) {
				jumpingTime = 5;
				Jump();
				jumpCount--;

				app->audio->PlayFx(jumpSFX);
			}

			onGround = false;
		}
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {

			longPress = true;

			if (onGround == true) {
				jumping = true;
				jumpingTime = 0;

				app->audio->PlayFx(jumpSFX);
			}

			onGround = false;
		}

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {

			isFliped = true;

			velocity.x = -5;

			if (isFliped == true && fliped == SDL_FLIP_NONE) {
				fliped = SDL_FLIP_HORIZONTAL;
			}
			currentAnim = &runPlayer;

		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			isFliped = false;
	

			velocity.x = 5;

			if (isFliped == false && fliped == SDL_FLIP_HORIZONTAL) {
				fliped = SDL_FLIP_NONE;
			}
			currentAnim = &runPlayer;

		}
		if (app->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN 
			|| app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) 
		{
			ResetPlayerPos();
			app->audio->PlayFx(selectSFX);
		}


		//Jumping Function
		if (jumping == true && jumpingTime <= 12) {
			Jump();
		}


		longPress = false;
	}

	pbody->body->SetLinearVelocity(velocity);
	
	// Link player's texture with pbody when moving, if player's dies then stop motion
	if (dead == true) {
		currentAnim = &diePlayer;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);
		pbody->body->SetAwake(false);
	}
	else {
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x - (width / 4));
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y - (height / 3));
	}
	
	SDL_Rect rect = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect, fliped);
	currentAnim->Update();
	

	return true;
}

bool Player::PostUpdate() {

	
	return true;
}

bool Player::CleanUp()
{

	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	// L07 DONE 7: Detect the type of collision

	switch (physB->cType)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		break;
	case ColliderType::COIN:
		LOG("Collision COIN");
		coins++;
		app->audio->PlayFx(pickCoinSFX);
		LOG("COINS: %d", coins);
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		jumpVel = GRAVITY_Y;
		onGround = true;
		jumping = false;
		jumpCount = 2;
		break;
	case ColliderType::WATER:
		LOG("Collision WATER");
		if(godMode == false)
			dead = true;
			coins = 0;
			app->audio->PlayFx(dieSFX);
			app->fade->FadeToBlack((Module*)this, (Module*)app->endingscreen, 30);
		break;
	case ColliderType::WIN_ZONE:
		LOG("Collision WIN ZONE");
		app->fade->FadeToBlack((Module*)this, (Module*)app->titlescreen, 90);
		app->audio->PlayFx(levelCompletedSFX);
		break;
	case ColliderType::ENEMY:
		LOG("Collision ENEMY");
		break;
	case ColliderType::WALL:
		LOG("Collision WALL");
		break;
	case ColliderType::CAMERAFIX:
		LOG("Collision CameraFix");
		app->scene->cameraFix = true;
		break;
	case ColliderType::NONCAMERAFIX:
		LOG("Collision CameraFix");
		app->scene->cameraFix = false;
		break;
	case ColliderType::CAMERAFIX_2:
		LOG("Collision CameraFix 2");
		app->scene->cameraFix2 = true;
		break;
	case ColliderType::NONCAMERAFIX_2:
		LOG("Collision CameraFix 2");
		app->scene->cameraFix2 = false;
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}

}

void Player::Jump() {
	velocity.y = +jumpVel;

	//Mini Jump
	if (longPress == true)
		jumpVel -= 1.5f;
	else if(jumpCount == 0)
		jumpVel -= 5.0f;
	else
		jumpVel += 2.0f;

	jumpingTime++;
}

void Player::ResetPlayerPos() {

	pbody->body->SetSleepingAllowed(false);
	velocity = { 0, 0 };
	pbody->body->SetTransform(PIXEL_TO_METERS(startPos), 0.0f);
	app->scene->cameraFix2 = false;
	app->scene->cameraFix = false;
	app->render->camera.x = 0;
	dead = false;
	
	LOG("--RESETING PLAYER--");
}