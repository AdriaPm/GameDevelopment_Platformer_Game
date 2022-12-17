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

	//origintexturePath = config.child("originTexture").attribute("origintexturePath").as_string();
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
	app->scene->slime->lives = 2;

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

	origintexturePath = app->configNode.child("scene").child("originTexture").attribute("origintexturePath").as_string();

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
			app->render->camera.x = (-player->position.x) + (app->win->screenSurface->w) / 2;
		else
			app->render->camera.x = 0;
	}

	if (cameraFix == true) 
	{
		if (cameraFix2 == true)
			app->render->camera.x = -2336;  // 2236 = map width - window width 
		else
			app->render->camera.x = (-player->position.x-23) + (app->win->screenSurface->w) / 2;
	}

	// God Mode key
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		player->godMode = !player->godMode;
		app->audio->PlayFx(selectSFX);
	}

	// Cap FPS to 30
	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
		capTo30fps = !capTo30fps;

	// Draw map
	app->map->Draw();

	//Blit UI
	app->ui->BlitCoins();
	app->ui->BlitLives();
	app->ui->BlitFPS();

	if (app->physics->debug) {
		app->ui->BlitPlayerXPos();
		app->ui->BlitPlayerYPos();
		app->ui->BlitSlimeLives();
		app->ui->BlitAverageFPS();
		app->ui->BlitDT();
		app->ui->BlitTimeSinceStart();
		app->ui->BlitFrameCount();
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

	/* app->entityManager->Disable();
	app->pathfinding->Disable();
	app->physics->Disable();
	app->map->CleanUp();*/


	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return true;
}

void Scene::ResetScene() {
	
	app->audio->PlayMusic("Assets/Audio/Music/song1.ogg", 1.0f);
	player->ResetPlayerPos();
	player->lives = 3;
	
	//coin->ResetCoin();
}

bool Scene::LoadState(pugi::xml_node& data)
{
	// Load previous saved player position
	b2Vec2 playerPos = { data.child("playerPosition").attribute("x").as_float(), data.child("playerPosition").attribute("y").as_float() };
	app->scene->player->pbody->body->SetTransform(playerPos, 0);
	
	// Load previous saved cameraFix & cameraFix2 parameters
	app->scene->cameraFix = data.child("cameraIsFix").attribute("value").as_bool();
	app->scene->cameraFix2 = data.child("cameraIsFix2").attribute("value").as_bool();

	//Load previous saved player number of lives
	app->scene->player->lives = data.child("playerLives").attribute("playerLives").as_int();


	// Load previous saved slime position
	b2Vec2 slimePos = { data.child("slimePosition").attribute("x").as_float(), data.child("slimePosition").attribute("y").as_float() };
	app->scene->slime->pbody->body->SetTransform(slimePos, 0);

	//Load previous saved slime number of lives
	app->scene->slime->lives = data.child("slimeLives").attribute("slimeLives").as_int();

	return true;
}

bool Scene::SaveState(pugi::xml_node& data)
{
	// Save current player position
	pugi::xml_node playerPos = data.append_child("playerPosition");
	playerPos.append_attribute("x") = app->scene->player->pbody->body->GetTransform().p.x;
	playerPos.append_attribute("y") = app->scene->player->pbody->body->GetTransform().p.y;

	// Save cameraFix parameter
	pugi::xml_node cameraIsFix = data.append_child("cameraIsFix");
	cameraIsFix.append_attribute("value") = app->scene->cameraFix;

	// Save cameraFix2 parameter
	pugi::xml_node cameraIsFix2 = data.append_child("cameraIsFix2");
	cameraIsFix2.append_attribute("value") = app->scene->cameraFix2;

	// Save current player number of lives
	pugi::xml_node playerLives = data.append_child("playerLives");
	playerLives.append_attribute("playerLives") = app->scene->player->lives;


	// Save current slime position
	pugi::xml_node slimePos = data.append_child("slimePosition");
	slimePos.append_attribute("x") = app->scene->slime->pbody->body->GetTransform().p.x;
	slimePos.append_attribute("y") = app->scene->slime->pbody->body->GetTransform().p.y;

	// Save current slime number of lives
	pugi::xml_node slimeLives = data.append_child("slimeLives");
	slimeLives.append_attribute("slimeLives") = app->scene->slime->lives;

	return true;
}