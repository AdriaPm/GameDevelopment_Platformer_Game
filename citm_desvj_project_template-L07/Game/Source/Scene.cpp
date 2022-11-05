#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "Physics.h"
#include "ModuleFadeToBlack.h"


#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	
	name.Create("scene");
	
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{

	LOG("Loading Scene");
	bool ret = true;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}

	//L02: DONE 3: Instantiate the player using the entity manager
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = config.child("player");


	return ret;
}

// Called before the first frame
bool Scene::Start()
{	

	// Play level music
	app->audio->PlayMusic("Assets/Audio/Music/song1.ogg", 1.0f);

	// Loading set of SFX
	selectSFX = app->audio->LoadFx("Assets/Audio/Fx/select.wav");
	
	// L03: DONE: Load map
	app->map->Load();

	SString title("Misco Jones Adventures by ADVENTURES JOKES STUDIO");

	/*
	// L04: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());
	*/

	app->win->SetTitle(title.GetString());

	
	app->entityManager->Enable();
	app->physics->Enable();
	app->scene->player->ResetPlayerPos();
	//player->pbody->body->SetType(b2_dynamicBody);
	//player->position = player->startingPosition;

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	// L03: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	/*
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += 1;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= 1;
	*/

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += 5;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= 5;

	// Camera movement related to player's movement
	if (cameraFix2 == true)
	{
		if (cameraFix == true)
			app->render->camera.x = -player->position.x + (app->win->screenSurface->w) / 2;
		else
			app->render->camera.x = 0;
	}

	if (cameraFix == true) 
	{
		if (cameraFix2 == true)
			app->render->camera.x = -2336;  // 2236 = map width - window width 
		else
			app->render->camera.x = -player->position.x + (app->win->screenSurface->w) / 2;
	}

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN){
		player->godMode = !player->godMode;
		app->audio->PlayFx(selectSFX);
	}

	//app->render->DrawTexture(img, 380, 100); // Placeholder not needed any more

	// Draw map
	app->map->Draw();

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	app->scene->Disable();

	return true;
}

void Scene::ResetScene() {
	player->Awake();
	item->Awake();
	player->ResetPlayerPos();
}