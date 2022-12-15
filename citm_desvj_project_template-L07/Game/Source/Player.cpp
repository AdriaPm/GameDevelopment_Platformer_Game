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
	startPos.x = parameters.attribute("x").as_int();
	startPos.y = parameters.attribute("y").as_int();

	texturePath = parameters.attribute("texturepath").as_string();

	width = 32;
	height = 32;

	idlePlayer.PushBack({ 0, 0, 65, 33 });
	idlePlayer.PushBack({ 64, 0, 65, 33 });
	idlePlayer.PushBack({ 128, 0, 65, 33 });
	idlePlayer.PushBack({ 192, 0, 65, 33 });
	idlePlayer.PushBack({ 256, 0, 65, 33 });
	idlePlayer.PushBack({ 319, 0, 65, 33 });
	idlePlayer.PushBack({ 384, 0, 65, 33 });
	idlePlayer.PushBack({ 448, 0, 65, 33 });
	idlePlayer.loop = true;
	idlePlayer.speed = 0.1f;
	
	runPlayer.PushBack({ 0, 32, 65, 33 });
	runPlayer.PushBack({ 64, 32, 65, 33 });
	runPlayer.PushBack({ 128, 32, 65, 33 });
	runPlayer.PushBack({ 192, 32, 65, 33 });
	runPlayer.PushBack({ 256, 32, 65, 33 });
	runPlayer.PushBack({ 319, 32, 65, 33 });
	runPlayer.PushBack({ 384, 32, 65, 33 });
	runPlayer.PushBack({ 448, 32, 65, 33 });
	runPlayer.loop = true;
	runPlayer.speed = 0.3f;

	attackPlayer.PushBack({ 0, 128, 65, 33 });
	attackPlayer.PushBack({ 64, 128, 65, 33 });
	attackPlayer.PushBack({ 128, 128, 65, 33 });
	attackPlayer.PushBack({ 192, 128, 65, 33 });
	attackPlayer.PushBack({ 256, 128, 65, 33 });
	attackPlayer.PushBack({ 319, 128, 65, 33 });
	attackPlayer.PushBack({ 384, 128, 65, 33 });
	attackPlayer.PushBack({ 448, 128, 65, 33 });
	attackPlayer.loop = false;
	attackPlayer.speed = 0.3f;

	diePlayer.PushBack({ 23, 192, 65, 33 });
	diePlayer.PushBack({ 64, 192, 65, 33 });
	diePlayer.PushBack({ 128, 192, 65, 33 });
	diePlayer.PushBack({ 192, 192, 65, 33 });
	diePlayer.PushBack({ 256, 192, 65, 33 });
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
	shortRangeAttackSFX = app->audio->LoadFx("Assets/Audio/Fx/whip_attack.wav");

	currentAnim = &idlePlayer;
	dead = false;

	//Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircle(position.x, position.y, width / 3, bodyType::DYNAMIC, ColliderType::PLAYER);

	pbody->listener = this;

	onGround = true;
	jumping = false;
	jumpCount = 2;

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
				jumpingTime = 0;
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

		//Reset player position input
		if (app->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN
			|| app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		{
			ResetPlayerPos();
			app->audio->PlayFx(selectSFX);
		}



		//Attacking animation function
		if (app->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
		{
			if (timeToAttack >= cooldownTime) 
			{
				attacking = true;
				app->audio->PlayFx(shortRangeAttackSFX);
			}

		}
		Attack();
		timeToAttack++;

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
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x - (width));
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y - (height/1.5));
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
		if (app->input->GetKey(SDL_SCANCODE_SPACE) != KEY_REPEAT) {
			onGround = true;
			jumping = false;
		}
		jumpCount = 2;
		break;
	case ColliderType::WATER:
		LOG("Collision WATER");
		if (godMode == false) {
			dead = true;
			coins = 0;
			app->audio->PlayFx(dieSFX);
			app->fade->FadeToBlack((Module*)app->scene, (Module*)app->endingscreen, 60);
		}
		break;
	case ColliderType::WIN_ZONE:
		LOG("Collision WIN ZONE");
		app->audio->PlayFx(levelCompletedSFX);
		app->fade->FadeToBlack((Module*)app->scene, (Module*)app->titlescreen, 90);
		break;
	case ColliderType::ENEMY:
		LOG("Collision ENEMY");
		break;
	case ColliderType::SLIME_HITBOX:
		LOG("Collison SLIME HEAD HITBOX");
		app->scene->slime->lives--;
		if (app->scene->slime->lives <= 0) {
			app->scene->slime->dead = true;
		}
		app->audio->PlayFx(app->scene->slime->stompSFX);
		break;
	case ColliderType::WALL:
		LOG("Collision WALL");
		break;
	case ColliderType::CAMERAFIX:
		LOG("Collision CameraFix");
		app->scene->cameraFix = true;
		break;
	case ColliderType::NONCAMERAFIX:
		LOG("Collision NONCameraFix");
		app->scene->cameraFix = false;
		break;
	case ColliderType::CAMERAFIX_2:
		LOG("Collision CameraFix 2");
		app->scene->cameraFix2 = true;
		break;
	case ColliderType::NONCAMERAFIX_2:
		LOG("Collision NONCameraFix 2");
		app->scene->cameraFix2 = false;
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}

}

void Player::Jump() {
	velocity.y = jumpVel;

	//Mini Jump
	if (longPress == true)
		jumpVel = -15.0f;
	else if(jumpCount == 0)
		jumpVel = -20.0f;
	else
		jumpVel = -5.0f;

	jumpingTime++;
}

void Player::Attack() {

	if (attacking)
	{
		currentAnim = &attackPlayer;

		if (attackTime > 0)
		{
			if (fliped == false)
				hitbox = app->physics->CreateRectangleSensor(METERS_TO_PIXELS(pbody->body->GetTransform().p.x) + 15, METERS_TO_PIXELS(pbody->body->GetTransform().p.y), 35, 2, bodyType::STATIC, ColliderType::PLAYER_ATTACK_HITBOX);

			if (fliped == true)
				hitbox = app->physics->CreateRectangleSensor(METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 15, METERS_TO_PIXELS(pbody->body->GetTransform().p.y), 35, 2, bodyType::STATIC, ColliderType::PLAYER_ATTACK_HITBOX);

		}
		attackTime--;

		if (attackPlayer.HasFinished()) {
			attacking = false;
			attackPlayer.Reset();
			attackPlayer.ResetLoopCount();
			timeToAttack = 0;
			attackTime = 1;

			app->physics->world->DestroyBody(hitbox->body);
		}
	}
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