#include "TitleScreen.h"

#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
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
bool Scene::Start()
{
	img = app->tex->Load("Assets/Textures/TitleScreen.png");
	

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
	// L03: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		app->fade->FadeToBlack(this, (Module*)app->scene, 90);

	return true;
}

// Called each loop iteration
bool TitleScreen::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(img, 0, 0, NULL);

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool TitleScreen::CleanUp()
{
	LOG("Freeing scene");

	if (img != nullptr) {
		app->tex->UnLoad(img);
	}

	return true;
}
