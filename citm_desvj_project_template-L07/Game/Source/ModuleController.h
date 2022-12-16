#ifndef _MODULECONTROLLER_H_
#define _MODULECONTROLLER_H_

#include "SDL/include/SDL.h"
#include "Module.h"
#include "Animation.h"
#include "Point.h"

#define MAX_BUTTONS 150
#define DEATHZONE  15000
#define MAX_GAME_CONTROLLERS 2

class ModuleController : public Module
{
public:
  
    ModuleController();
    
    // Destructor
    virtual ~ModuleController();

    // Called before render is available
    bool Awake(pugi::xml_node& config);

    // Called before the first frame
    bool Start();

    // Called before all Updates
    bool PreUpdate();

    // Called each loop iteration
    bool Update();

    // Called before all Updates
    bool PostUpdate();

    // Called before quitting
    bool CleanUp();

public:

    SDL_Event event;

    //GAME CONTROLLER 1

    SDL_GameController* Controller_player1 = nullptr;

    bool Controller_player1_Connected = false;                //controller connected?
    bool controller_player1_A_pressed = false;                //button A pressed?
    bool controller_player1_RightShoulder_pressed = false;    //button RightShoulder pressed?
    bool controller_player1_Start_pressed = false;            //button Start pressed?
    bool controller_player1_B_pressed = false;                //button B pressed?
    bool controller_player1_UPArrow_pressed = false;          //button UP Arrow pressed?
    bool controller_player1_DOWNArrow_pressed = false;        //button DOWN Arrow pressed?
    bool controller_player1_LEFTArrow_pressed = false;        //button LEFT Arrow pressed?
    bool controller_player1_RIGHTArrow_pressed = false;       //button RIGHT Arrow pressed?

    //values for the Joysticks Axes
    int Controller_player1_LAxisX = 0;
    int Controller_player1_LAxisY = 0;

    //=========================================

    //GAME CONTROLLER 2

    SDL_GameController* Controller_player2 = nullptr;

    bool Controller_player2_Connected = false;                //controller connected?
    bool controller_player2_A_pressed = false;                //button A pressed?
    bool controller_player2_RightShoulder_pressed = false;    //button RightShoulder pressed?
    bool controller_player2_Start_pressed = false;            //button Start pressed?
    bool controller_player2_B_pressed = false;                //button B pressed?
    bool controller_player2_UPArrow_pressed = false;          //button UP Arrow pressed?
    bool controller_player2_DOWNArrow_pressed = false;        //button DOWN Arrow pressed?
    bool controller_player2_LEFTArrow_pressed = false;        //button LEFT Arrow pressed?
    bool controller_player2_RIGHTArrow_pressed = false;       //button RIGHT Arrow pressed?

    //values for the Joysticks Axes
    int Controller_player2_LAxisX = 0;
    int Controller_player2_LAxisY = 0;

};
#endif