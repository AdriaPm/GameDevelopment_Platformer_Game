#ifndef __ENDINGSCREEN_H__
#define __ENDINGCREEN_H__

#include "Module.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class EndingScreen : public Module
{
public:

	EndingScreen();

	// Destructor
	virtual ~EndingScreen();

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
	uint startSFX = 0;

private:
	SDL_Texture* img = nullptr;

	const char* imgPath;
	const char* musicPath;
	const char* startSFXPath;

};


#endif // !__ENDINGSCREEN_H__