#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "Physics.h"
#include "Pathfinding.h"
#include "ModuleFadeToBlack.h"
#include "LogoScreen.h"
#include "TitleScreen.h"
#include "EndingScreen.h"
#include "UI.h"
#include "Fonts.h"
#include "ModuleController.h"
#include "GuiManager.h"
#include "GuiButton.h"
#include "GuiControl.h"

#include "Defs.h"
#include "Log.h"
#include "Optick/include/optick.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;

	input = new Input();
	win = new Window();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	controller = new ModuleController();
	fade = new ModuleFadeToBlack();
	logoscreen = new LogoScreen();
	titlescreen = new TitleScreen();
	//L07 TODO 2: Add Physics module
	physics = new Physics();
	pathfinding = new PathFinding();
	scene = new Scene();
	fonts = new Fonts();
	ui = new UI();
	entityManager = new EntityManager();
	map = new Map();
	endingscreen = new EndingScreen();
	guiManager = new GuiManager();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input, true);
	AddModule(win, true);
	AddModule(tex, true);
	AddModule(audio, true);
	AddModule(controller, true);
	AddModule(fade, true);
	AddModule(logoscreen, true);
	AddModule(titlescreen, false);
	//L07 TODO 2: Add Physics module
	AddModule(physics, false);
	AddModule(pathfinding, false);
	AddModule(scene, false);
	AddModule(fonts, true);
	AddModule(ui, true);
	AddModule(entityManager, false);
	AddModule(map, false);
	AddModule(endingscreen, false);
	AddModule(guiManager, true);

	// Render last to swap buffer
	AddModule(render, true);
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();
}

void App::AddModule(Module* module, bool activate)
{
	if(activate == true)
		module->Init();

	modules.Add(module);
}

// Called before render is available
bool App::Awake()
{
	// L13: TODO 3: Measure the amount of ms that takes to execute the Awake and LOG the result
	timer = Timer();

	bool ret = false;

	// L01: DONE 3: Load config from XML
	ret = LoadConfig();

	if (ret == true)
	{
		title = configNode.child("app").child("title").child_value(); // L01: DONE 4: Read the title from the config file

		// L14: TODO 1: Read from config file your framerate cap
		
			maxFrameDuration = configNode.child("app").child("frcap").attribute("value").as_int();
	
		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			// L01: DONE 5: Add a new argument to the Awake method to receive a pointer to an xml node.
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			pugi::xml_node node = configNode.child(item->data->name.GetString());
			ret = item->data->Awake(node);
			item = item->next;
		}
	}

	LOG("---------------- Time Awake: %f/n", timer.ReadMSec());

	return ret;
}

// Called before the first frame
bool App::Start()
{
	// L13: TODO 3: Measure the amount of ms that takes to execute the App Start() and LOG the result
	timer.Start();
	startupTime.Start();
	lastSecFrameTime.Start();

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->Start();
	}

	LOG("----------------- Time Start(): %f", timer.ReadMSec());

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if (app->scene->capTo30fps == false)
		maxFrameDuration = configNode.child("app").child("frcap").attribute("value").as_int();
	else if (app->scene->capTo30fps == true)
		maxFrameDuration = configNode.child("app").child("frcap").attribute("value2").as_int();

	if (input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if (ret == true)
		ret = PreUpdate();

	if (ret == true)
		ret = DoUpdate();

	if (ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// Load config from XML file
bool App::LoadConfig()
{
	bool ret = false;

	// L01: DONE 3: Load config.xml file using load_file() method from the xml_document class
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");

	// L01: DONE 3: Check result for loading errors
	if (parseResult) {
		ret = true;
		configNode = configFile.child("config");
	}
	else {
		LOG("Error in App::LoadConfig(): %s", parseResult.description());
	}

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	frameTime.Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	OPTICK_CATEGORY("FinishUpdate", Optick::Category::GameLogic);
	// L03: DONE 1: This is a good place to call Load / Save methods
	if (loadGameRequested == true) LoadFromFile();
	if (saveGameRequested == true) SaveToFile();

	// L13: TODO 4: Now calculate:
	// Amount of frames since startup
	frameCount++;
	// Amount of time since game start (use a low resolution timer)
	secondsSinceStartup = startupTime.ReadSec();
	// Amount of ms took the last update
	dt = frameTime.ReadMSec();
	// Amount of frames during the last second
	lastSecFrameCount++;

	if (lastSecFrameTime.ReadMSec() > 1000) {
		lastSecFrameTime.Start();
		framesPerSecond = lastSecFrameCount;
		lastSecFrameCount = 0;
		// Average FPS for the whole game life
		averageFps = (averageFps + framesPerSecond) / 2;
	}

	// L14: TODO 2: Use SDL_Delay to make sure you get your capped framerate
	// L14: TODO 3: Measure accurately the amount of time SDL_Delay() actually waits compared to what was expected

	float delay = float(maxFrameDuration) - dt;

	PerfTimer delayTimer = PerfTimer();
	delayTimer.Start();
	if (maxFrameDuration > 0 && delay > 0 && render->limitFPS == false) {
		SDL_Delay(delay);
		LOG("We waited for %f milliseconds and the real delay is % f", delay, delayTimer.ReadMs());
		dt = maxFrameDuration;
	}
	else {
		//LOG("No wait");
	}

	// Shows the time measurements in the window title
	static char title[256];
	sprintf_s(title, 256, "FPS: %i | Avg.FPS: %.2f | Last dt: %.3f | V-Sync: %s",
		framesPerSecond, averageFps, dt, app->render->limitFPS ? "ON" : "OFF");
	app->win->SetTitle(title);

}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	OPTICK_CATEGORY("PreUpdate", Optick::Category::GameLogic);
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	OPTICK_CATEGORY("DoUpdate", Optick::Category::GameLogic);
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	OPTICK_CATEGORY("PostUpdate", Optick::Category::GameLogic);
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}


	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if (index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

// ---------------------------------------
uint App::GetFPS() 
{
	return framesPerSecond;
}

// ---------------------------------------
float App::GetAverageFPS()
{
	return averageFps;
}

// ---------------------------------------
float App::GetDT()
{
	return dt;
}

// ---------------------------------------
float App::GetTimesSinceStart()
{
	return secondsSinceStartup;
}

// ---------------------------------------
uint App::GetFrameCount()
{
	return frameCount;
}


// L02: DONE 1: Implement methods to request load / save and methods 
// for the real execution of load / save (to be implemented in TODO 5 and 7)
void App::LoadGameRequest()
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist
	loadGameRequested = true;
}

// ---------------------------------------
void App::SaveGameRequest() 
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist and... should we overwriten
	saveGameRequested = true;
}


// L02: DONE 5: Implement the method LoadFromFile() to actually load a xml file
// then call all the modules to load themselves
bool App::LoadFromFile()
{
	bool ret = true;

	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");

	if (result == NULL)
	{
		LOG("Could not load xml file savegame.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			ret = item->data->LoadState(gameStateFile.child("save_state").child(item->data->name.GetString()));
			item = item->next;
		}
	}

	loadGameRequested = false;

	return ret;
}

// L02: DONE 7: Implement the xml save method SaveToFile() for current state
// check https://pugixml.org/docs/quickstart.html#modify
bool App::SaveToFile() 
{
	bool ret = false;

	pugi::xml_document* saveDoc = new pugi::xml_document();
	pugi::xml_node saveStateNode = saveDoc->append_child("save_state");

	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL)
	{
		ret = item->data->SaveState(saveStateNode.append_child(item->data->name.GetString()));
		item = item->next;
	}

	ret = saveDoc->save_file("save_game.xml");

	saveGameRequested = false;

	return ret;
}

pugi::xml_node App::LoadConfigFileToVar()
{
	pugi::xml_node ret;
	// L01: DONE 3: Load config.xml file using load_file() method from the xml_document class
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");

	// L01: DONE 3: Check result for loading errors
	if (parseResult) {
		ret = configFile.child("config");
	}
	else {
		LOG("Error in App::LoadConfig(): %s", parseResult.description());
	}

	return ret;
}