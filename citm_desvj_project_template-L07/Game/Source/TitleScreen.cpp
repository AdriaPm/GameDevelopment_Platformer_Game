#include "TitleScreen.h"

#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Entity.h"
#include "Physics.h"
#include "EntityManager.h"
#include "Scene.h"
#include "Map.h"
#include "ModuleFadeToBlack.h"
#include "GuiManager.h"

#include "SDL/include/SDL_render.h"

#include "Defs.h"
#include "Log.h"

TitleScreen::TitleScreen() : Module()
{
	name.Create("TitleScreen");
}

// Destructor
TitleScreen::~TitleScreen()
{}

// Called before render is available
bool TitleScreen::Awake(pugi::xml_node& config)
{
	LOG("Loading TitleScreen");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool TitleScreen::Start()
{
	app->render->camera.x = 0;

	app->map->Disable();

	LOG("--STARTS TITLE SCENE--");

	/*Initialize*/
	imgPath = app->configNode.child("title").child("backgroundimage").attribute("texturepath").as_string();
	musicPath = app->configNode.child("title").child("music").attribute("musicPath").as_string();
	startSFXPath = app->configNode.child("title").child("startsfx").attribute("startSFXPath").as_string();

	/*Load*/
	img = app->tex->Load(imgPath);
	app->audio->PlayMusic(musicPath);
	startSFX = app->audio->LoadFx(startSFXPath);

	app->physics->debug = false;

	// L15: TODO 2: Declare a GUI Button and create it using the GuiManager
	uint w, h;
	app->win->GetWindowSize(w, h);
	playButton1 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Play", { ((int)w / 2) - (93/2), (int(h)-200), 93, 29}, this);
	settingsButton2 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Setting", { ((int)w / 2) - (93 / 2), (int(h) - 170), 93, 29 }, this);
	creditsButton3 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Credits", { ((int)w / 2) - (93 / 2), (int(h) - 140), 93, 29 }, this);
	exitButton4 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "Exit", { ((int)w / 2) - (93 / 2), (int(h) - 110), 93, 29 }, this);

	continueButton5 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "Continue", { ((int)w / 2) - (93 / 2), (int(h) - 240), 93, 29 }, this);

	return true;
}

// Called each loop iteration
bool TitleScreen::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool TitleScreen::Update(float dt)
{
		
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		LOG("PASA A GAME SCENE");
		app->fade->FadeToBlack(this, (Module*)app->scene, 90);
		app->audio->PlayFx(startSFX);
	}
	
	app->render->DrawTexture(img, 0, 0, NULL);

	//Draw GUI
	app->guiManager->Draw();

	return true;
}

// Called each loop iteration
bool TitleScreen::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (exitGame == true)
		ret = false;

	return ret;
}

// Called before quitting
bool TitleScreen::CleanUp()
{
	LOG("Freeing TITLE SCENE");

	if (img != nullptr) {
		app->tex->UnLoad(img);
	}
	
	//app->guiManager->guiControlsList.Clear();

	//delete(playButton1);

	return true;
}

bool TitleScreen::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: TODO 5: Implement the OnGuiMouseClickEvent method

	switch (control->id)
	{
	case 1:
		// Play button
		app->fade->FadeToBlack(this, (Module*)app->scene, 90);
		app->audio->PlayFx(startSFX);
		break;

	case 4:
		// Exit button
		exitGame = !exitGame;
		break;

	default:
		break;
	}

	return true;
}