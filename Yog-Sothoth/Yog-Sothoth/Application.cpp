#include "Globals.h"
#include "Application.h"

#include <list>
#include "Tools/Static/JsonSerializer.h"


Application::Application()
{
	outputStream = new std::ostringstream();
	inputStream = new std::istringstream();
	//log.setStream(*outputStream);
	//console.setConsoleInput(*inputStream);
	//console.setConsoleOutput(log);

	fs = new M_FileSystem();
	window = new M_Window();
	renderer = new M_Renderer();
	uiManager = new M_UIManager();
	input = new M_Input();	
	resourceManager = new M_ResourceManager();
	objManager = new M_ObjectManager();

	addModule(fs);
	addModule(window);	
	addModule(input);
	addModule(uiManager);
	addModule(resourceManager);
	addModule(objManager);
	
	//LAST ONE
	addModule(renderer);
}
Application::~Application()
{

}
bool Application::Init()
{
	bool ret = true;
	
	fs->Init();
	LoadConfig();

	std::list<Module*>::iterator item = list_modules.begin();

	while (item != list_modules.end() && ret == true)
	{
		if ((*item)->IsEnabled() && (*item)->name.compare("filesystem") != 0)
			ret = (*item)->Init();
		item++;
	}

	// After all Init calls we call Start() in all modules
	SDL_Log("-------------- Application Start --------------");
	item = list_modules.begin();

	while (item != list_modules.end() && ret == true)
	{
		if ((*item)->IsEnabled())
			ret = (*item)->Start();
		item++;
	}

	loadHardwareInfo();

	gameStatus = game_status::STOP;
	appTimer.Stop();

	return ret;
}

void Application::PrepareUpdate()
{
	updateGameStatus();
	appTimer.OnPrepareUpdate(gameStatus);
}

void Application::FinishUpdate()
{
	appTimer.OnFinishUpdate();
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	float dt = appTimer.GetDT();
	std::list<Module*>::iterator item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		if ((*item)->IsEnabled())
		{
			ret = (*item)->PreUpdate(dt);
		}
		++item;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		if ((*item)->IsEnabled())
		{
			ret = (*item)->Update(dt);
		}
		++item;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		if ((*item)->IsEnabled())
		{
			ret = (*item)->PostUpdate(dt);
		}
		++item;
	}

	FinishUpdate();

	if (MaxFPS > 0 && (appTimer.LastFrameMs() < (1000 / MaxFPS)))
	{
		SDL_Delay((1000 / MaxFPS) - appTimer.LastFrameMs());
	}

	return ret;
}
bool Application::CleanUp()
{
	bool ret = true;
	for (auto element : list_modules)
	{
		RELEASE(element);
	}

	appTimer.Stop();
	return ret;
}
bool Application::OpenBrowser(const char* link)
{
	ShellExecute(nullptr, "open", link, nullptr, nullptr, SW_SHOWNORMAL);
	return true;
}
void Application::Log(char* str)
{
	
}
char * Application::getOrganization()
{
	return "PlaceHolder";
}
char * Application::getTitle()
{
	return "PlaceHolder";
}

void Application::addModule(Module* module)
{
	list_modules.push_back(module);
}

void Application::removeModule(Module* module)
{
	std::find(list_modules.begin(), list_modules.end(), module);
}
void Application::Serialize(Json::Value& root)
{
	root["name"] = appName.data();
	root["organization"] = organization.data();
	root["max_fps"] = MaxFPS;
	Json::Value paths;
	for (std::list<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); ++it)
	{
		paths[(*it)->name] = configPath[(*it)->name].c_str();
	}

	root["config_paths"] = paths;
}
/**
 * \brief Fills configPath with the paths inside config.json
 * \param root Parent node of the parsed jsonFile
 */
void Application::Deserialize(Json::Value& root)
{	
	organization = root.get("organization", "Org not loaded").asString();
	appName = root.get("name", "AppName not loaded").asString();
	MaxFPS = root.get("max_fps", 0).asFloat();

	Json::Value paths = root["config_paths"];
	Json::Value::Members members =  paths.getMemberNames();
	for (int i = 0; i < members.size(); ++i)
	{
		configPath[members[i].c_str()] = paths[members[i]].asString();
	}


}
void Application::LoadConfig()
{
	JsonSerializer::DeserializeFormPath(this, "config.json");
}
void Application::SaveConfig()
{
	std::string output;
	JsonSerializer::Serialize(this, output, "config.json");
	SDL_Log("%s", output);
}

void Application::loadHardwareInfo()
{
	hardwareInfo.platform = SDL_GetPlatform();
	SDL_VERSION(&hardwareInfo.sdlVersion);
	hardwareInfo.numCPUs = SDL_GetCPUCount();
	hardwareInfo.cache = SDL_GetCPUCacheLineSize();
	hardwareInfo.systemRAM = SDL_GetSystemRAM();
	hardwareInfo.DNow = SDL_Has3DNow();
	hardwareInfo.AVX = SDL_HasAVX();
	hardwareInfo.AVX2 = SDL_HasAVX2();
	hardwareInfo.altiVec = SDL_HasAltiVec();
	hardwareInfo.MMX = SDL_HasMMX();
	hardwareInfo.RDTSC = SDL_HasRDTSC();
	hardwareInfo.SSE = SDL_HasSSE();
	hardwareInfo.SSE2 = SDL_HasSSE2();
	hardwareInfo.SSE3 = SDL_HasSSE3();
	hardwareInfo.SSE41 = SDL_HasSSE41();
	hardwareInfo.SSE42 = SDL_HasSSE42();
}

void Application::updateGameStatus()
{
	if (gameStatus == game_status::TO_PLAY)
	{
		objManager->SaveScene("data/library/defaultScene.scene");
		gameStatus = game_status::PLAY;
	}
	if (gameStatus == game_status::TO_STOP)
	{
		objManager->LoadScenePrefab("data/library/defaultScene.scene");
		gameStatus = game_status::STOP;
	}
}