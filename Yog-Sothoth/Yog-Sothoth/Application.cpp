#include "Globals.h"
#include "Application.h"

#include <list>
#include "Tools/Static/JsonSerializer.h"


Application::Application()
{
	outputStream = new std::ostringstream();
	inputStream = new std::istringstream();
	log.setStream(*outputStream);
	console.setConsoleInput(*inputStream);
	console.setConsoleOutput(log);

	window = new M_Window();
	renderer = new M_Renderer();
	uiManager = new M_UIManager();
	input = new M_Input();
	fs = new M_FileSystem();
	
	addModule(fs);
	addModule(window);	
	addModule(input);
	addModule(uiManager);
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
	return ret;
}
void Application::PrepareUpdate()
{
}
void Application::FinishUpdate()
{
}
update_status Application::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();

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

	return ret;
}
bool Application::CleanUp()
{
	bool ret = true;
	for (auto element : list_modules)
	{
		RELEASE(element);
	}
	return ret;
}
bool Application::OpenBrowser(const char* link)
{
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
}
/**
 * \brief Fills configPath with the paths inside config.json
 * \param root Parent node of the parsed jsonFile
 */
void Application::Deserialize(Json::Value& root)
{	
	organization = root.get("organization", "Org not loaded").asString();
	appName = root.get("name", "AppName not loaded").asString();

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
