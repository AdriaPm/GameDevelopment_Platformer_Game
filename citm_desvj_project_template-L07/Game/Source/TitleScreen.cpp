#include "TitleScreen.h"

#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Entity.h"
#include "Physics.h"
#include "EntityManager.h"
#include "Scene.h"
#include "Map.h"
#include "ModuleFadeToBlack.h"

#include "SDL/include/SDL_render.h"

#include "Defs.h"
#include "Log.h"

TitleScreen::TitleScreen() : Module()
{
	name.Create("TitleScreen");
}

// Destructor
TitleScreen::~TitleScreen()
{}

// Called before render is available
bool TitleScreen::Awake(pugi::xml_node& config)
{
	LOG("Loading TitleScreen");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool TitleScreen::Start()
{
	app->render->camera.x = 0;

	app->map->Disable();

	LOG("--STARTS TITLE SCENE--");

	/*Initialize*/
	imgPath = app->configNode.child("title").child("backgroundimage").attribute("texturepath").as_string();
	musicPath = app->configNode.child("title").child("music").attribute("musicPath").as_string();
	startSFXPath = app->configNode.child("title").child("startsfx").attribute("startSFXPath").as_string();

	/*Load*/
	img = app->tex->Load(imgPath);
	app->audio->PlayMusic(musicPath);
	startSFX = app->audio->LoadFx(startSFXPath);

	app->physics->debug = false;

	return true;
}

// Called each loop iteration
bool TitleScreen::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool TitleScreen::Update(float dt)
{
	
	
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		LOG("PASA A GAME SCENE");
		app->fade->FadeToBlack(this, (Module*)app->scene, 90);
		app->audio->PlayFx(startSFX);
	}
	
	app->render->DrawTexture(img, 0, 0, NULL);

	return true;
}

// Called each loop iteration
bool TitleScreen::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool TitleScreen::CleanUp()
{
	LOG("Freeing TITLE SCENE");

	if (img != nullptr) {
		app->tex->UnLoad(img);
	}

	return true;
}
