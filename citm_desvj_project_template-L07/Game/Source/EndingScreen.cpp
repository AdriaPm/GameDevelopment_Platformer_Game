#include "EndingScreen.h"

#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Entity.h"
#include "Physics.h"
#include "EntityManager.h"
#include "ModuleFadeToBlack.h"
#include "Player.h"
#include "Map.h"
#include "Scene.h"

#include "SDL/include/SDL_render.h"

#include "Defs.h"
#include "Log.h"

EndingScreen::EndingScreen() : Module()
{
	name.Create("EndingScreen");
}

// Destructor
EndingScreen::~EndingScreen()
{}

// Called before render is available
bool EndingScreen::Awake(pugi::xml_node& config)
{
	LOG("Loading EndingScreen");
	bool ret = true;


	return ret;
}

// Called before the first frame
bool EndingScreen::Start()
{
	LOG("--STARTS ENDING SCENE--");
	/*app->entityManager->Disable();
	app->scene->Disable();
	app->physics->Disable();
	app->map->Disable();*/

	img = app->tex->Load("Assets/Textures/EndingScreen.png");
	app->scene->player->dead = false;
	app->scene->player->ResetPlayerPos();
	startSFX = app->audio->LoadFx("Assets/Audio/Fx/start_game.wav");
	app->audio->PlayMusic("Assets/Audio/Music/gameOver.ogg");

	//app->scene->player->godMode = true;

	return true;
}

// Called each loop iteration
bool EndingScreen::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool EndingScreen::Update(float dt)
{
	

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		LOG("PASA A GAME SCENE");
		app->fade->FadeToBlack(this, (Module*)app->scene, 90);
		//app->scene->cameraFix = false;
		app->audio->PlayFx(startSFX);
	}

	return true;
}

// Called each loop iteration
bool EndingScreen::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	app->render->DrawTexture(img, app->render->camera.x, 0, NULL);

	return ret;
}

// Called before quitting
bool EndingScreen::CleanUp()
{
	LOG("Freeing ENDING SCENE");
	//app->scene->Enable();
	if (img != nullptr) {
		app->tex->UnLoad(img);
	}

	return true;
}
