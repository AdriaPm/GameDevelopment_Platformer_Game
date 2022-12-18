#ifndef __BATENEMY_H__
#define __BATENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "Physics.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class BatEnemy : public Entity
{
public:

	BatEnemy();

	virtual ~BatEnemy();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update();

	bool PostUpdate();

	bool CleanUp();

	void MovementDirection(const iPoint& origin, const iPoint& destination);

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void ResetBat();

public:

	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation* currentAnim;

	//Set of animations
	Animation flyingEnemy;
	Animation dieEnemy;
	Animation hitEnemy;

	//Set of SFX
	uint stompSFX = 0;
	const char* stompSFXPath;
	uint powerUpSFX = 0;
	const char* powerUpSFXPath;
	uint batHitSFX = 0;
	const char* batHitSFXPath;

	bool onGround = false;
	bool dead = false;
	bool onCollision = false;

	PhysBody* pbody;

	PhysBody* hitbox;
	b2Vec2 hitboxPos;

	int lives = 2;

private:

	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

	SDL_RendererFlip fliped = SDL_FLIP_NONE;

	b2Vec2 velocity;
	b2Vec2 startPos;

	int width;
	int height;

	// L12: Debug pathfing
	iPoint origin;
	bool originSelected = false;
	uint refreshPathTime;
};
#endif // __ENEMY_H__

