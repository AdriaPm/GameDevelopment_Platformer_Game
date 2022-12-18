#ifndef __LOGOSCREEN_H__
#define __LOGOSCREEN_H__

#include "Module.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class LogoScreen : public Module
{
public:

	LogoScreen();

	// Destructor
	virtual ~LogoScreen();

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

public:

private:
	SDL_Texture* img = nullptr;
	int time = 0;

	const char* imgPath;
	const char* musicPath;

	// A frame count system to handle the fade time and ratio
	Uint32 frameCount = 250;
	Uint32 maxFadeFrames = 250;
	float fadeRatio;
};


#endif // !__LOGOSCREEN_H__


