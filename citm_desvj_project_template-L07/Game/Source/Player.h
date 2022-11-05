#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "Physics.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update();

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void Jump();

	void ResetPlayerPos();

public:

	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation* currentAnim;

	//Set of animations
	Animation idlePlayer;
	Animation runPlayer;
	Animation jumpPlayer;
	Animation diePlayer;

	//Set of SFX
	uint jumpSFX = 0;
	uint dieSFX = 0;
	uint pickCoinSFX = 0;
	uint levelCompletedSFX = 0;
	uint selectSFX = 0;

	bool onGround = false;
	bool dead = false;
	bool godMode = false;

	PhysBody* pbody;

private:

	// L07 TODO 5: Add physics to the player - declare a Physics body
	

	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

	SDL_RendererFlip fliped = SDL_FLIP_NONE;
	
	b2Vec2 velocity;
	b2Vec2 startPos;

	int width;
	int height;

	bool isFliped = false;

	//Jump Variables
	int jumpingTime;
	uint jumpCount = 1;
	float jumpVel = GRAVITY_Y;
	bool jumping = false;
	bool longPress = false;

	int coins = 0;
};

#endif // __PLAYER_H__