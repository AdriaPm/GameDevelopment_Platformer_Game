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
	app->map->Disable();
	app->scene->Disable();
	app->entityManager->Disable();
	app->physics->Disable();	

	LOG("Loading EndingScreen");
	bool ret = true;


	return ret;
}

// Called before the first frame
bool EndingScreen::Start()
{
	img = app->tex->Load("Assets/Textures/EndingScreen.png");

	//app->audio->PlayMusic("Assets/Audio/Music/menu_music.ogg");

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
	

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		LOG("PASA A OTRA PUTA ESCENA");
		app->fade->FadeToBlack(this, (Module*)app->scene, 90);
	}

	//app->render->DrawTexture(img, app->render->viewport.x, app->render->viewport.y, NULL);

	return true;
}

// Called each loop iteration
bool EndingScreen::PostUpdate()
{
	bool ret = true;

	//app->render->DrawTexture(img, 0, 0, NULL);

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;
	

	// Render GAME OVER image
	app->render->DrawTexture(img, (app->scene->player->position.x)-((app->win->screenSurface->w) / 2), 0, NULL);

	return ret;
}

// Called before quitting
bool EndingScreen::CleanUp()
{
	LOG("Freeing scene");

	if (img != nullptr) {
		app->tex->UnLoad(img);
	}

	return true;
}
