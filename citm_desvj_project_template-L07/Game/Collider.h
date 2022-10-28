#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "SDL/include/SDL_Rect.h"

#define MAX_LISTENERS 20

class Module;

struct Collider
{
	enum Type
	{
		NONE = -1,
		UPPER_WALL,
		LOWER_WALL,
		PLAYER,
		ENEMY,
		PLAYER_SHOT,
		ENEMY_SHOT,
		LEFT_3P_GOAL,
		RIGHT_3P_GOAL,
		LEFT_5P_GOAL,
		RIGHT_5P_GOAL,
		NET,
		DISK,
		PARABOLIC_DISK,
		FLOOR,


		MAX
	};

	//Methods
	Collider(SDL_Rect rectangle, Type type, Module* listener = nullptr);

	void SetPos(int x, int y);

	bool Intersects(const SDL_Rect& r) const;

	void AddListener(Module* listener);

	//Variables
	SDL_Rect rect;
	bool pendingToDelete = false;
	Type type;
	Module* listeners[MAX_LISTENERS] = { nullptr };
};


#endif // !__COLLIDER_H__