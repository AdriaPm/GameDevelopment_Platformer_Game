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
#include "Fonts.h"
#include "UI.h"

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
	popImgSettingsPath = app->configNode.child("title").child("popImage").attribute("settingtexturepath").as_string();
	popImgCreditsPath = app->configNode.child("title").child("popImage").attribute("creditstexturepath").as_string();
	musicPath = app->configNode.child("title").child("music").attribute("musicPath").as_string();
	startSFXPath = app->configNode.child("title").child("startsfx").attribute("startSFXPath").as_string();
	selectSFXPath = app->configNode.child("title").child("selectsfx").attribute("selectSFXPath").as_string();

	/*Load*/
	img = app->tex->Load(imgPath);
	popImg_settings = app->tex->Load(popImgSettingsPath);
	popImg_credits = app->tex->Load(popImgCreditsPath);
	app->audio->PlayMusic(musicPath);
	startSFX = app->audio->LoadFx(startSFXPath);
	menuSelectionSFX = app->audio->LoadFx(selectSFXPath);

	app->physics->debug = false;
	settingMenu = false;
	creditsMenu = false;

	// L15: TODO 2: Declare a GUI Button and create it using the GuiManager
	uint w, h;
	app->win->GetWindowSize(w, h);
	playButton1 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "play", 5, { ((int)w / 2) - (93/2), (int(h)-200), 93, 29}, this);
	settingsButton2 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "setting", 8, { ((int)w / 2) - (93 / 2), (int(h) - 170), 93, 29 }, this);
	creditsButton3 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "credits", 8,{ ((int)w / 2) - (93 / 2), (int(h) - 140), 93, 29 }, this);
	exitButton4 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "exit", 5,{ ((int)w / 2) - (93 / 2), (int(h) - 110), 93, 29 }, this);
	//continueButton5 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "continue", 9,{ ((int)w / 2) - (93 / 2), (int(h) - 240), 93, 29 }, this);
	closeSettingMenuButton6 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, "close", 6, { 780, 110, 93, 29 }, this);
	closeCreditsMenuButton7 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, "close", 6, { 780, 110, 93, 29 }, this);

	decreaseMusicButton8 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 8, "decrease", 9, { 378, 243, 93, 29 }, this);
	increaseMusicButton9 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 9, "increase", 9, { 555, 243, 93, 29 }, this);

	decreaseSFXButton10 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10, "decrease", 9, { 378, 322, 93, 29 }, this);
	increaseSFXButton11 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, "increase", 9, { 555, 322, 93, 29 }, this);

	fullscreenButton12 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 12, "fullscreen", 11, { 466, 420, 93, 29 }, this);

	vsyncButton13 = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 13, "vsync", 6, { 466, 502, 93, 29 }, this);



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

	// Setting Menu
	closeSettingMenuButton6->state = GuiControlState::DISABLED;
	decreaseMusicButton8->state = GuiControlState::DISABLED;
	increaseMusicButton9->state = GuiControlState::DISABLED;
	decreaseSFXButton10->state = GuiControlState::DISABLED;
	increaseSFXButton11->state = GuiControlState::DISABLED;
	fullscreenButton12->state = GuiControlState::DISABLED;
	vsyncButton13->state = GuiControlState::DISABLED;

	if (settingMenu == true)
	{
		playButton1->state = GuiControlState::DISABLED;
		settingsButton2->state = GuiControlState::DISABLED;
		creditsButton3->state = GuiControlState::DISABLED;
		exitButton4->state = GuiControlState::DISABLED;

		app->render->DrawTexture(popImg_settings, 0, 0, NULL);
	
		closeSettingMenuButton6->state = GuiControlState::NORMAL;
		decreaseMusicButton8->state = GuiControlState::NORMAL;
		increaseMusicButton9->state = GuiControlState::NORMAL;
		decreaseSFXButton10->state = GuiControlState::NORMAL;
		increaseSFXButton11->state = GuiControlState::NORMAL;
		fullscreenButton12->state = GuiControlState::NORMAL;
		vsyncButton13->state = GuiControlState::NORMAL;

		char music[10];
		sprintf_s(music, 10, "%d", app->musicValue);
		app->fonts->BlitText(500, 250, app->ui->font1_id, music);

		char sfx[10];
		sprintf_s(sfx, 10, "%d", app->sfxValue);
		app->fonts->BlitText(500, 330, app->ui->font1_id, sfx);

		char fullscreen[10];
		sprintf_s(fullscreen, 10, "%s", app->win->fullscreenMode ? "on" : "off");
		app->fonts->BlitText(502, 428, app->ui->font1_id, fullscreen);

		char vsync[10];
		sprintf_s(vsync, 10, "%s", app->render->limitFPS ? "on" : "off");
		app->fonts->BlitText(502, 510, app->ui->font1_id, vsync);
	}

	// Credits Menu
	closeCreditsMenuButton7->state = GuiControlState::DISABLED;

	if (creditsMenu == true)
	{
		playButton1->state = GuiControlState::DISABLED;
		settingsButton2->state = GuiControlState::DISABLED;
		creditsButton3->state = GuiControlState::DISABLED;
		exitButton4->state = GuiControlState::DISABLED;

		app->render->DrawTexture(popImg_credits, 0, 0, NULL);

		closeCreditsMenuButton7->state = GuiControlState::NORMAL;
	}

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

	//app->guiManager->guiControlsList.Clear();

	if (img != nullptr) {
		app->tex->UnLoad(img);
		app->tex->UnLoad(popImg_settings);
		app->tex->UnLoad(popImg_credits);
	}
	
	playButton1->state = GuiControlState::DISABLED;
	settingsButton2->state = GuiControlState::DISABLED;
	creditsButton3->state = GuiControlState::DISABLED;
	exitButton4->state = GuiControlState::DISABLED;
	//continueButton5->state = GuiControlState::DISABLED;
	closeSettingMenuButton6->state = GuiControlState::DISABLED;
	closeCreditsMenuButton7->state = GuiControlState::DISABLED;
	decreaseMusicButton8->state = GuiControlState::DISABLED;
	increaseMusicButton9->state = GuiControlState::DISABLED;
	decreaseSFXButton10->state = GuiControlState::DISABLED;
	increaseSFXButton11->state = GuiControlState::DISABLED;
	fullscreenButton12->state = GuiControlState::DISABLED;
	vsyncButton13->state = GuiControlState::DISABLED;

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

	case 2:
	case 6:
		// Settings button
		settingMenu = !settingMenu;
		if (settingMenu == false) 
		{
			playButton1->state = GuiControlState::NORMAL;
			settingsButton2->state = GuiControlState::NORMAL;
			creditsButton3->state = GuiControlState::NORMAL;
			exitButton4->state = GuiControlState::NORMAL;
		}
		app->audio->PlayFx(menuSelectionSFX);
		break;

	case 3:
	case 7:
		// Credits button
		creditsMenu = !creditsMenu;
		if (creditsMenu == false)
		{
			playButton1->state = GuiControlState::NORMAL;
			settingsButton2->state = GuiControlState::NORMAL;
			creditsButton3->state = GuiControlState::NORMAL;
			exitButton4->state = GuiControlState::NORMAL;
		}
		app->audio->PlayFx(menuSelectionSFX);
		break;

	case 8:
		// Decrease music volume
		app->musicValue = app->musicValue - 1;
		if (app->musicValue <= 0)
			app->musicValue = 0;
		if (app->musicValue >= 100)
			app->musicValue = 100;
		Mix_VolumeMusic(app->musicValue);
		app->audio->PlayFx(menuSelectionSFX);
		break;

	case 9:
		// Increase music volume
		app->musicValue = app->musicValue + 1;
		if (app->musicValue <= 0)
			app->musicValue = 0;
		if (app->musicValue >= 100)
			app->musicValue = 100;
		Mix_VolumeMusic(app->musicValue);
		app->audio->PlayFx(menuSelectionSFX);
		break;

	case 10:
		// Decrease SFX volume
		app->sfxValue = app->sfxValue - 1;
		if (app->sfxValue <= 0)
			app->sfxValue = 0;
		if (app->sfxValue >= 100)
			app->sfxValue = 100;
		Mix_Volume(-1, app->sfxValue);
		app->audio->PlayFx(menuSelectionSFX);
		break;

	case 11:
		// Increase SFX volume
		app->sfxValue = app->sfxValue + 1;
		if (app->sfxValue <= 0)
			app->sfxValue = 0;
		if (app->sfxValue >= 100)
			app->sfxValue = 100;
		Mix_Volume(-1, app->sfxValue);
		app->audio->PlayFx(menuSelectionSFX);
		break;

	case 12:
		// Fullscreen button
		app->win->fullscreenMode = !app->win->fullscreenMode;
		if (app->win->fullscreenMode == true)
		{
			SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
		}
		else if(app->win->fullscreenMode == false)
		{
			SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_SHOWN);
		}
		app->audio->PlayFx(menuSelectionSFX);
		break;

	case 13:
		// V-Sync button
		app->render->limitFPS = !app->render->limitFPS;
		app->audio->PlayFx(menuSelectionSFX);
		break;

	case 4:
		// Exit button
		exitGame = !exitGame;
		app->audio->PlayFx(menuSelectionSFX);
		break;


	default:
		break;
	}

	return true;
}