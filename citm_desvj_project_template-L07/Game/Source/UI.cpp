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
	
	/*
	font_id = app->font->Load("Assets/Textures/font.png", "0123456789", 1);

	char lookupTable[] = { "!  ,_./0123456789$;<&?abcdefghijklmnopqrstuvwxyz" };
	font2_id = app->font->Load("Assets/Textures/font2.png", "! @,_./0123456789$;<&?abcdefghijklmnopqrstuvwxyz", 2);
	*/

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

	//app->font->UnLoad(font_id);

	return true;
}

/*
void ModuleUI::BlitPlayerXPos() {
	char playerXPos[25];
	sprintf_s(playerXPos, 25, "position x; %d", App->player->position.x);
	App->font->BlitText(465, 743, font2_id, playerXPos);
}

void ModuleUI::BlitPlayerYPos() {
	char playerYPos[25];
	sprintf_s(playerYPos, 25, "position y; %d", App->player->position.y);
	App->font->BlitText(465, 753, font2_id, playerYPos);
}
*/