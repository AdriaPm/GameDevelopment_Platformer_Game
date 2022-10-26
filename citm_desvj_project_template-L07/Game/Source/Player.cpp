#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

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
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
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

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	currentAnim = &idlePlayer;

	// L07 TODO 5: Add physics to the player - initialize physics body

	pbody = app->physics->CreateCircle(position.x, position.y, width / 2.5, bodyType::DYNAMIC);

	return true;
}

bool Player::Update()
{
	// L07 TODO 5: Add physics to the player - updated player position using physics
	b2Vec2 velocity;
	velocity = { 0, -GRAVITY_Y }; // Players default velocity (when any key is pressed)

	//L02: DONE 4: modify the position of the player using arrow keys and render the texture
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		//position.y -= 1;
	
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		//position.y += 1;

	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		//position.x -= 1;
		isFliped = true;

		velocity = { -5, -GRAVITY_Y };

		if (isFliped == true && fliped == SDL_FLIP_NONE) {
			fliped = SDL_FLIP_HORIZONTAL;
			LOG("FLIPED");
		}
		//currentAnim = &runPlayer;
		
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		//position.x += 1;		
		
		velocity = { 5, -GRAVITY_Y };
		//currentAnim = &runPlayer;
		/*if (isFliped)
			fliped = SDL_FLIP_HORIZONTAL;*/

	}
	pbody->body->SetLinearVelocity(velocity);

	// Link player's texture with pbody when moving
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x - (width / 2));
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y - (height / 2));

	SDL_Rect rect = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect, playerSpeed, angle, pivotX, pivotY, fliped);
	currentAnim->Update();

	return true;
}

bool Player::CleanUp()
{

	return true;
}