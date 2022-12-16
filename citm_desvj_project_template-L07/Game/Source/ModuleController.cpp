#include "ModuleController.h"

#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Render.h"
#include "Audio.h"
#include "Physics.h"
#include "ModuleFadeToBlack.h"
#include "Fonts.h"
#include "UI.h"

#include <SDL/include/SDL.h>

#include "Defs.h"
#include "Log.h"

ModuleController::ModuleController():Module()
{
	name.Create("Controller_Module");
}

ModuleController::~ModuleController() 
{}

bool ModuleController::Awake(pugi::xml_node& config)
{
	LOG("Loading UI_Module");
	bool ret = true;



	return ret;
}

bool ModuleController::Start()
{
	LOG("--STARTS CONTROLLER MODULE--");

	SDL_Init(SDL_INIT_GAMECONTROLLER);

	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
		LOG("Error, gamecontroller fail to initilize", SDL_GetError());

	//SDL_GameControllerAddMappingsFromFile(""); -> for adding controll mapping

	return true;
}

bool ModuleController::PreUpdate() 
{
	//Controller inputs

	for (int i = 0; i < SDL_NumJoysticks(); ++i)
	{
		if (SDL_IsGameController(i))
		{
			if (i == 0)
			{
				Controller_player1 = SDL_GameControllerOpen(i);
				//Checking if the controller is attached

				if (SDL_GameControllerGetAttached(Controller_player1))
				{
					//Assign values of the aixis to Controller_Player1_LAxisX...
					Controller_player1_LAxisX = SDL_GameControllerGetAxis(Controller_player1, SDL_CONTROLLER_AXIS_LEFTX);
					Controller_player1_LAxisY = SDL_GameControllerGetAxis(Controller_player1, SDL_CONTROLLER_AXIS_LEFTY);

					//Assign the boolean value to the  bools defined  
					controller_player1_A_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_A);
					//controller_player1_RightShoulder_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
					controller_player1_Start_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_START);
					controller_player1_B_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_B); ;
					controller_player1_UPArrow_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_DPAD_UP);
					controller_player1_DOWNArrow_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
					controller_player1_LEFTArrow_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
					controller_player1_RIGHTArrow_pressed = SDL_GameControllerGetButton(Controller_player1, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

					Controller_player1_Connected = true;
				}
				else
				{
					SDL_GameControllerClose(Controller_player1);
					Controller_player1 = nullptr;
					Controller_player1_Connected = false;
				}
			}
			else if (i < 1)
			{
				Controller_player2_Connected = false;
				SDL_GameControllerClose(Controller_player2);
				Controller_player2 = nullptr;

			}
			else if (i == 1 || i == 0 && Controller_player1_Connected == false)
			{
				Controller_player2 = SDL_GameControllerOpen(i);
				if (SDL_GameControllerGetAttached(Controller_player2)) {

					Controller_player2_LAxisX = SDL_GameControllerGetAxis(Controller_player2, SDL_CONTROLLER_AXIS_LEFTX);
					Controller_player2_LAxisY = SDL_GameControllerGetAxis(Controller_player2, SDL_CONTROLLER_AXIS_LEFTY);

					//Assign the boolean value to the  bools defined  

					controller_player2_A_pressed = SDL_GameControllerGetButton(Controller_player2, SDL_CONTROLLER_BUTTON_A);

					/*	controller_player2_RightShoulder_pressed = SDL_GameControllerGetButton(Controller_player2, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);*/
					controller_player2_Start_pressed = SDL_GameControllerGetButton(Controller_player2, SDL_CONTROLLER_BUTTON_START);
					controller_player2_B_pressed = SDL_GameControllerGetButton(Controller_player2, SDL_CONTROLLER_BUTTON_B);
					controller_player2_UPArrow_pressed = SDL_GameControllerGetButton(Controller_player2, SDL_CONTROLLER_BUTTON_DPAD_UP);
					controller_player2_DOWNArrow_pressed = SDL_GameControllerGetButton(Controller_player2, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
					controller_player2_LEFTArrow_pressed = SDL_GameControllerGetButton(Controller_player2, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
					controller_player2_RIGHTArrow_pressed = SDL_GameControllerGetButton(Controller_player2, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

					Controller_player2_Connected = true;
					break;
				}
				else
				{
					SDL_GameControllerClose(Controller_player2);
					Controller_player2 = nullptr;
					Controller_player2_Connected = false;
				}
			}
		}
	}
	//if (App->input->keys[SDL_SCANCODE_LEFT] == Key_State::KEY_REPEAT) { block.inputX = -1; }

	//Check player 1 Left Axis X & Y
	if (Controller_player1_LAxisX > 6400)
	{
		app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT;
	}
	else if (Controller_player1_LAxisX < -DEATHZONE)
	{
		app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT;
	}

	if (Controller_player1_LAxisY < -DEATHZONE)
	{
		app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT;
	}
	else if (Controller_player1_LAxisY > DEATHZONE)
	{
		app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT;
	}

	//Check controller player 1 buttons
	if (controller_player1_A_pressed == true && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE)
	{
		app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN;
	}
	else if (controller_player1_A_pressed == true)
	{
		app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT;
	}

	if (controller_player1_Start_pressed == true && app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_IDLE)
	{
		app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN;
	}
	else if (controller_player1_Start_pressed == true)
	{
		app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_REPEAT;
	}

	if (controller_player1_B_pressed == true && app->input->GetKey(SDL_SCANCODE_C) == KEY_IDLE)
	{
		app->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN;
	}
	else if (controller_player1_B_pressed == true)
	{
		app->input->GetKey(SDL_SCANCODE_C) == KEY_REPEAT;
	}

	if (controller_player1_UPArrow_pressed == true && app->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE)
	{
		app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN;
	}
	else if (controller_player1_UPArrow_pressed == true)
	{
		app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT;
	}

	if (controller_player1_DOWNArrow_pressed == true && app->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE)
	{
		app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN;
	}
	else if (controller_player1_DOWNArrow_pressed == true)
	{
		app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT;
	}

	if (controller_player1_LEFTArrow_pressed == true && app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE)
	{
		app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN;
	}
	else if (controller_player1_LEFTArrow_pressed == true)
	{
		app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT;
	}

	if (controller_player1_RIGHTArrow_pressed == true && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
	{
		app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN;
	}
	else if (controller_player1_RIGHTArrow_pressed == true)
	{
		app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT;
	}

	//Check player 2 axes
	if (Controller_player2_LAxisX > DEATHZONE)
	{
		app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT;
	}
	else if (Controller_player2_LAxisX < -DEATHZONE)
	{
		app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT;
	}

	if (Controller_player2_LAxisY < -DEATHZONE)
	{
		app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT;
	}
	else if (Controller_player2_LAxisY > DEATHZONE)
	{
		app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT;
	}

	//Check controller player 2 buttons
	if (controller_player2_A_pressed == true && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE)
	{
		app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN;
	}
	else if (controller_player2_A_pressed == true)
	{
		app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT;
	}

	if (controller_player2_Start_pressed == true && app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_IDLE)
	{
		app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN;
	}
	else if (controller_player2_Start_pressed == true)
	{
		app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_REPEAT;
	}

	if (controller_player2_B_pressed == true && app->input->GetKey(SDL_SCANCODE_N) == KEY_IDLE)
	{
		app->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN;
	}
	else if (controller_player2_B_pressed == true)
	{
		app->input->GetKey(SDL_SCANCODE_N) == KEY_REPEAT;
	}

	if (controller_player2_UPArrow_pressed == true && app->input->GetKey(SDL_SCANCODE_UP) == KEY_IDLE)
	{
		app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN;
	}
	else if (controller_player2_UPArrow_pressed == true)
	{
		app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT;
	}

	if (controller_player2_DOWNArrow_pressed == true && app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_IDLE)
	{
		app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN;
	}
	else if (controller_player2_DOWNArrow_pressed == true)
	{
		app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT;
	}

	if (controller_player2_LEFTArrow_pressed == true && app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE)
	{
		app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN;
	}
	else if (controller_player2_LEFTArrow_pressed == true)
	{
		app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT;
	}

	if (controller_player2_RIGHTArrow_pressed == true && app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE)
	{
		app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN;
	}
	else if (controller_player2_RIGHTArrow_pressed == true)
	{
		app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT;
	}

	return true;
}

bool ModuleController::Update()
{
	
	return true;
}

bool ModuleController::PostUpdate()
{

	return true;
}

// Called before quitting
bool  ModuleController::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);

	SDL_GameControllerClose(Controller_player1);
	SDL_GameControllerClose(Controller_player2);

	Controller_player1 = nullptr;
	Controller_player2 = nullptr;

	return true;
}