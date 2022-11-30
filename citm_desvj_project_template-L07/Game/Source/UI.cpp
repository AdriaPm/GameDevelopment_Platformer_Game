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


void UI::BlitCoins()
{
	char playerCoins[20];
	sprintf_s(playerCoins, 20, "coins; %d", app->scene->player->coins);
	app->fonts->BlitText(20, 20, font1_id, playerCoins);
}

void UI::BlitPlayerXPos() 
{
	char playerXPos[25];
	sprintf_s(playerXPos, 25, "position x; %d", app->scene->player->position.x);
	app->fonts->BlitText(20, 30, font1_id, playerXPos);
}

void UI::BlitPlayerYPos() 
{
	char playerYPos[25];
	sprintf_s(playerYPos, 25, "position y; %d", app->scene->player->position.y);
	app->fonts->BlitText(20, 40, font1_id, playerYPos);
}