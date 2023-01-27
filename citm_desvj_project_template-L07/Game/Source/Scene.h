#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "Coin.h"
#include "SlimeEnemy.h"
#include "BatEnemy.h"
#include "GuiButton.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	void ResetScene();

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

public:

	//L02: DONE 3: Declare a Player attribute 
	Player* player;

	SlimeEnemy* slime;

	BatEnemy* bat;

	Item* item;

	Coin* coin;

	// Set of SFX
	uint selectSFX = 0;

	bool cameraFix = false;
	bool cameraFix2 = false;

	bool playing = false;

	bool capTo30fps = false;

	SDL_Texture* originTex = nullptr;
	const char* origintexturePath;

	SDL_Texture* slimeTilePathTex = nullptr;
	const char* slimeTilePath;

	GuiButton* btn1;

	bool gamePaused = false;

private:
	SDL_Texture* img;
	iPoint startPosition;
	
	// L12: Debug pathfing
	iPoint origin;
	bool originSelected = false;

	const char* musicPath;
	const char* selectSFXPath;

	// Declare a GUI Button and create it using the GuiManager
	//GuiButton* btn1;
	GuiButton* btn2;
};

#endif // __SCENE_H__