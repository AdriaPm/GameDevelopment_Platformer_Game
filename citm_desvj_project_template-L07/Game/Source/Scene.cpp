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
#include "PathFinding.h"
#include "ModuleFadeToBlack.h"
#include "EndingScreen.h"
#include "UI.h"

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

	origintexturePath = config.child("originTexture").attribute("origintexturePath").as_string();
	slimeTilePath = config.child("pathfinding").attribute("slimePathTile").as_string();
 

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("coin"); itemNode; itemNode = itemNode.next_sibling("coin"))
	{
		coin = (Coin*)app->entityManager->CreateEntity(EntityType::COIN);
		coin->parameters = itemNode;
	}

	//L02: DONE 3: Instantiate the player using the entity manager
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = config.child("player");

	slime = (SlimeEnemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
	slime->parameters = config.child("slime");

	return ret;
}

// Called before the first frame
bool Scene::Start()
{	
	app->physics->Enable();
	app->pathfinding->Enable();
	app->entityManager->Enable();
	app->map->Enable();
	LOG("--STARTS GAME SCENE--");
	app->physics->debug = false;

	// L03: DONE: Load map
	app->map->Load();
	


	// L12 Create walkability map
	if (app->map->Load()) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	// Play level music
	app->audio->PlayMusic("Assets/Audio/Music/song1.ogg", 1.0f);

	// Loading set of SFX
	selectSFX = app->audio->LoadFx("Assets/Audio/Fx/select.wav");
	
	// Texture to highligh mouse position 
	slimeTilePathTex = app->tex->Load(slimeTilePath);
	// Texture to show path origin 
	originTex = app->tex->Load(origintexturePath);

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

	ResetScene();

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
	{
		app->SaveGameRequest();
		app->audio->PlayFx(selectSFX);
	}
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		app->LoadGameRequest();
		app->audio->PlayFx(selectSFX);
	}

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

	// God Mode key
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		player->godMode = !player->godMode;
		app->audio->PlayFx(selectSFX);
	}

	// Draw map
	app->map->Draw();

	//Blit UI
	app->ui->BlitCoins();
	app->ui->BlitLives();

	if (app->physics->debug) {
		app->ui->BlitPlayerXPos();
		app->ui->BlitPlayerYPos();
	}

	//// L08: DONE 3: Test World to map method
	//int mouseX, mouseY;
	//app->input->GetMousePosition(mouseX, mouseY);
	//iPoint mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x,
	//										mouseY - app->render->camera.y);

	////Convert again the tile coordinates to world coordinates to render the texture of the tile
	//if (app->physics->debug) 
	//{
	//	iPoint highlightedTileWorld = app->map->MapToWorld(mouseTile.x, mouseTile.y);
	//	app->render->DrawTexture(slimeTilePathTex, highlightedTileWorld.x, highlightedTileWorld.y);
	//}

	////Test compute path function
	//if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	//{
	//	if (originSelected == true)
	//	{
	//		app->pathfinding->CreatePath(origin, mouseTile);
	//		originSelected = false;
	//	}
	//	else
	//	{
	//		origin = mouseTile;
	//		originSelected = true;
	//		app->pathfinding->ClearLastPath();
	//	}
	//}

	//
	//if (app->physics->debug) 
	//{
	//// L12: Get the latest calculated path and draw
	//	const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
	//	for (uint i = 0; i < path->Count(); ++i)
	//	{
	//	iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
	//	app->render->DrawTexture(slimeTilePathTex, pos.x, pos.y);
	//	}

	//	// L12: Debug pathfinding
	//	iPoint originScreen = app->map->MapToWorld(origin.x, origin.y);
	//	app->render->DrawTexture(originTex, originScreen.x - 16, originScreen.y - 19);
	//}


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
	LOG("Freeing GAME SCENE");

	return true;
}

void Scene::ResetScene() {
	
	app->audio->PlayMusic("Assets/Audio/Music/song1.ogg", 1.0f);
	player->ResetPlayerPos();
	player->lives = 3;
}