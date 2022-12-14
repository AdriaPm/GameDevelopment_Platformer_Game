#include "UI.h"

#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Input.h"
#include "Player.h"
#include "Scene.h"
#include "Window.h"

#include "SDL/include/SDL.h"

#include "Defs.h"
#include "Log.h"


UI::UI():Module() 
{
	name.Create("UI_Module");
}

UI::~UI()
{}

bool UI::Awake(pugi::xml_node& config) 
{
	LOG("Loading UI_Module");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool UI::Start()
{
	LOG("--STARTS UI MODULE--");
	
	//Loading font 1
	char lookupTableFont1[] = { "! @,_./0123456789$;<&?abcdefghijklmnopqrstuvwxyz" };
	font1_id = app->fonts->Load("Assets/Textures/font1.png", lookupTableFont1, 2);

	char lookupTableFont2[] = { "! %&'()*+,-./0123456789:;<=>abcdefghijklmnopqrstuvwxyz" };
	font2_id = app->fonts->Load("Assets/Textures/font2.png", lookupTableFont2, 1);

	return true;
}

// Called each loop iteration
bool UI::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool UI::Update(float dt)
{
	return true;
}

// Called each loop iteration
bool UI::PostUpdate()
{
	return true;
}

// Called before quitting
bool UI::CleanUp()
{
	LOG("Freeing UI_Module");

	app->fonts->UnLoad(font1_id);
	//app->fonts->UnLoad(font2_id);

	return true;
}


void UI::BlitLives()
{
	char playerLives[20];
	sprintf_s(playerLives, 20, "lives: %d", app->scene->player->lives);
	app->fonts->BlitText(20, 15, font2_id, playerLives);
}

void UI::BlitCoins()
{
	char playerCoins[20];
	sprintf_s(playerCoins, 20, "coins: %d", app->scene->player->coins);
	app->fonts->BlitText(20, 35, font2_id, playerCoins);
}


/* UI DEBUG INFO WITH FONT 2 */
void UI::BlitPlayerXPos()
{
	char playerXPos[25];
	sprintf_s(playerXPos, 25, "position x: %d", app->scene->player->position.x);
	app->fonts->BlitText(20, 55, font2_id, playerXPos);
}

void UI::BlitPlayerYPos()
{
	char playerYPos[25];
	sprintf_s(playerYPos, 25, "position y: %d", app->scene->player->position.y);
	app->fonts->BlitText(20, 75, font2_id, playerYPos);
}

void UI::BlitFPS()
{
	char fps[25];
	sprintf_s(fps, 25, "fps: %d", app->currentFPS);
	app->fonts->BlitText(900, 15, font2_id, fps);
}

/* UI DEBUG INFO WITH FONT 1 */
/*
void UI::BlitPlayerXPos() 
{
	char playerXPos[25];
	sprintf_s(playerXPos, 25, "position x; %d", app->scene->player->position.x);
	app->fonts->BlitText(20, 40, font1_id, playerXPos);
}

void UI::BlitPlayerYPos() 
{
	char playerYPos[25];
	sprintf_s(playerYPos, 25, "position y; %d", app->scene->player->position.y);
	app->fonts->BlitText(20, 50, font1_id, playerYPos);
}
*/