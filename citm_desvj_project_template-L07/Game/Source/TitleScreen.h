#ifndef __TITLESCREEN_H__
#define __TITLESCREEN_H__

#include "Module.h"
#include "SDL/include/SDL.h"
#include "GuiButton.h"

struct SDL_Texture;

class TitleScreen : public Module
{
public:

	TitleScreen();

	// Destructor
	virtual ~TitleScreen();

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

public:
	uint startSFX = 0;

	// Declare a GUI Button and create it using the GuiManager
	GuiButton* playButton1;
	GuiButton* settingsButton2;
	GuiButton* creditsButton3;
	GuiButton* exitButton4;
	GuiButton* continueButton5; // Show only if there is a saved game in "save_game.xml"

private:
	SDL_Texture* img = nullptr;

	const char* imgPath;
	const char* musicPath;
	const char* startSFXPath;

	bool exitGame = false;
};

#endif // !__TITLESCREEN_H__

