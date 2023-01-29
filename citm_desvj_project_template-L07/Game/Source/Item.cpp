#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "EntityManager.h"

Item::Item() : Entity(EntityType::ITEM)
{
	name.Create("item");
}

Item::~Item() {}

bool Item::Awake() {

	return true;
}

bool Item::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	iType = parameters.attribute("iType").as_string();

	width = 32;
	height = 32;

	//initilize textures
	texture = app->tex->Load(texturePath);
	
	if (iType == "life")
		lifeRect = {0, 0, 32, 32};

	// L07 TODO 4: Add a physics to an item - initialize the physics body
	pbody = app->physics->CreateCircleSensor(position.x, position.y, width/3, bodyType::KINEMATIC, ColliderType::ITEM);

	pbody->listener = this;

	return true;
}

bool Item::Update()
{
	// Link item's texture with pbody when moving

	if (app->scene->gamePaused != true)
	{
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
	}

	if (app->scene->gamePaused == true)
		pbody->body->SetLinearVelocity({ 0,0 });

	if (app->scene->gamePaused != true)
	{
		if (isPicked == false) {
			if (iType == "life")
				app->render->DrawTexture(texture, position.x, position.y, &lifeRect);
			else
				app->render->DrawTexture(texture, position.x, position.y);
		}
	}

	if (isPicked == true)
	{
		app->entityManager->DestroyEntity(this);
		app->physics->world->DestroyBody(pbody->body);
	}

	return true;
}

bool Item::PostUpdate()
{

	return true;
}

bool Item::CleanUp()
{
	return true;
}

void Item::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->cType)
	{
	case ColliderType::PLAYER:
		LOG("Collision PLAYER");


		if (iType == "life" && app->scene->player->lives < 3) {
			app->scene->itemLivesCount++;
			pbody->body->SetActive(false);
			this->Disable();
		}

		
		
		break;
	}

}

void Item::ResetItem() {
	SDL_SetTextureAlphaMod(texture, 1);
	pbody->body->SetActive(true);
	isPicked = false;
}