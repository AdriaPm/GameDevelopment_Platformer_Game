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

	void Attack();

	void ResetPlayerPos();

public:

	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation* currentAnim;

	//Set of animations (JUMP ANIM DDOESN'T EXISTS)
	Animation idlePlayer;
	Animation runPlayer;
	Animation attackPlayer;
	Animation diePlayer;
	Animation hitPlayer;

	//Set of SFX
	uint jumpSFX = 0;
	const char* jumpSFXPath;
	uint dieSFX = 0;
	const char* dieSFXPath;
	uint hurtSFX = 0;
	const char* hurtSFXPath;
	uint pickCoinSFX = 0;
	const char* pickCoinSFXPath;
	uint pickLifeSFX = 0;
	const char* pickLifeSFXPath;
	uint levelCompletedSFX = 0;
	const char* levelCompletedSFXPath;
	uint selectSFX = 0;
	const char* selectSFXPath;
	uint shortRangeAttackSFX = 0;
	const char* shortRangeAttackSFXPath;

	bool onGround = false;
	bool dead = false;
	bool godMode = false;
	bool onCollision = false;

	PhysBody* pbody;

	PhysBody* hitbox;
	b2Vec2 hitboxPos;

	int coins = 0;

	float lives = 3;

	//Attack Variables
	const int cooldownTime = 60;		//Cooldown time --> 60 ticks = 1 seconds (if fps=60)
	int timeToAttack = 60;				//Starts equal to cooldownTime, so player can attack immediately when the scene starts

	float gameTimer;

private:

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

	//Attack Variables
	bool attacking = false;
	int attackTime = 1;
};

#endif // __PLAYER_H__