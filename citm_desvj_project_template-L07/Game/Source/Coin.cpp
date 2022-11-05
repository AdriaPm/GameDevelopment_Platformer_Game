#include "Coin.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Coin::Coin() : Entity(EntityType::ITEM)
{
	name.Create("coin");
}

Coin::~Coin() {}

bool Coin::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	width = 32;
	height = 32;

	return true;
}

bool Coin::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	
	// L07 TODO 4: Add a physics to an item - initialize the physics body
	pbody = app->physics->CreateCircle(position.x, position.y, width/3, bodyType::KINEMATIC, ColliderType::COIN);

	pbody->listener = this;

	return true;
}

bool Coin::Update()
{
	// Link item's texture with pbody when moving
	

	if (timeMov <= 50)
		velocity.y = .5f;
	else if (timeMov <= 100) {
		velocity.y = -.5f;
	}
	else
		timeMov = 0;


	timeMov++;

	pbody->body->SetLinearVelocity(velocity);

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x - (width / 2));
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y - (height / 2));

	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Coin::PostUpdate()
{
	// L07 TODO 4: Add a physics to an item - update the position of the object from the physics.  
	

	return true;
}

bool Coin::CleanUp()
{
	return true;
}

void Coin::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->cType)
	{
	case ColliderType::PLAYER:
		LOG("Collision PLAYER");
		pbody->body->SetActive(false);
		this->Disable();
		break;
	}

}