#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "Physics.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

enum class EnemyType {
	NONE = -1,
	WALKING, 
	FLYING
};

class Enemy : public Entity
{
public:

	Enemy();

	virtual ~Enemy();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update();

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	// The pointer to the current player animation
	// It will be switched depending on the player's movement direction
	Animation* currentAnim;

	//Set of animations
	Animation idleEnemy;
	Animation runEnemy;
	Animation jumpEnemy;
	Animation dieEnemy;

	//Set of SFX
	uint jumpSFX = 0;
	uint dieSFX = 0;

	bool onGround = false;
	bool dead = false;

	PhysBody* pbody;

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
};
#endif // __ENEMY_H__
