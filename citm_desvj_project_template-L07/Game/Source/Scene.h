#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "Coin.h"
#include "SlimeEnemy.h"

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

	void ResetScene();

public:

	//L02: DONE 3: Declare a Player attribute 
	Player* player;

	SlimeEnemy* slime;

	Item* item;

	Coin* coin;

	// Set of SFX
	uint selectSFX = 0;

	bool cameraFix = false;
	bool cameraFix2 = false;

	bool playing = false;

	SDL_Texture* originTex = nullptr;
	const char* origintexturePath;

private:
	SDL_Texture* img;
	iPoint startPosition;

	SDL_Texture* mouseTileTex = nullptr;
	
	// L12: Debug pathfing
	iPoint origin;
	bool originSelected = false;
};

#endif // __SCENE_H__