#include "LogoScreen.h"

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

#include "SDL/include/SDL_render.h"

#include "Defs.h"
#include "Log.h"

LogoScreen::LogoScreen() : Module()
{
	name.Create("LogoScreen");
}

// Destructor
LogoScreen::~LogoScreen()
{}

// Called before render is available
bool LogoScreen::Awake(pugi::xml_node& config)
{
	app->entityManager->Disable();
	app->physics->Disable();
	LOG("Loading LogoScreen");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool LogoScreen::Start()
{
	img = app->tex->Load("Assets/Textures/LogoScreen.png");


	return true;
}

// Called each loop iteration
bool LogoScreen::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool LogoScreen::Update(float dt)
{


	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		LOG("PASA A OTRA PUTA ESCENA");
		app->fade->FadeToBlack(this, (Module*)app->titlescreen, 90);
	}
	app->render->DrawTexture(img, 0, 0, NULL);

	return true;
}

// Called each loop iteration
bool LogoScreen::PostUpdate()
{
	bool ret = true;

	//app->render->DrawTexture(img, 0, 0, NULL);

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;
	//app->render->DrawTexture(img, 0, 0, NULL);

	return ret;
}

// Called before quitting
bool LogoScreen::CleanUp()
{
	LOG("Freeing scene");

	if (img != nullptr) {
		app->tex->UnLoad(img);
	}

	return true;
}
