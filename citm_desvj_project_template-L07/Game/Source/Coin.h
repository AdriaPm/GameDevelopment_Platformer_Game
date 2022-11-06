#ifndef __COIN_H__
#define __COIN_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Coin : public Entity
{
public:

	Coin();
	virtual ~Coin();

	bool Awake();

	bool Start();

	bool Update();

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void ResetCoin();

public:

	bool isPicked = false;

	//TODO 4: Add a physics to an item
	PhysBody* pbody;

private:

	SDL_Texture* texture;
	const char* texturePath;

	b2Vec2 velocity;

	int timeMov = 0;

	int width;
	int height;

};

#endif // __ITEM_H__