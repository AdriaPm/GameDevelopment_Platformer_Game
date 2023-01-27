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
	LOG("Loading LogoScreen");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool LogoScreen::Start()
{
	LOG("--STARTS LOGO SCENE--");

	app->render->SetBackgroundColor({ 0,0,0,255 });

	/*Initialize*/
	imgPath = app->configNode.child("logo").child("backgroundimage").attribute("texturepath").as_string();
	musicPath = app->configNode.child("logo").child("music").attribute("musicPath").as_string();

	/*Load*/
	img = app->tex->Load(imgPath);
	app->audio->PlayMusic(musicPath);

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
	time++;
	frameCount--;

	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		LOG("PASA A SCENE DIRECTAMENTE");
		app->fade->FadeToBlack(this, (Module*)app->scene, 90);
	}

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		LOG("PASA A TITLE SCENE");
		app->fade->FadeToBlack(this, (Module*)app->titlescreen, 90);
	}

	fadeRatio = (float)frameCount / (float)maxFadeFrames;

	
	SDL_RenderFillRect(app->render->renderer, NULL);

	if (time >= 480) {
		LOG("PASA A TITLE SCENE");
		app->fade->FadeToBlack(this, (Module*)app->titlescreen, 90);
	}

	app->render->DrawTexture(img, 0, 0, NULL);

	return true;
}

// Called each loop iteration
bool LogoScreen::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
	SDL_RenderFillRect(app->render->renderer, NULL);

	return ret;
}

// Called before quitting
bool LogoScreen::CleanUp()
{
	LOG("Freeing LOGO SCENE");

	if (img != nullptr) {
		app->tex->UnLoad(img);
	}

	return true;
}
