#include "Globals.h"
#include "Application.h"

#include <list>

Application::Application()
{
	window = new M_Window();
	renderer = new M_Renderer();
	uiManager = new M_UIManager();
	input = new M_Input();
	
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
	// Call Init() in all modules
	std::list<Module*>::iterator item = list_modules.begin();

	while (item != list_modules.end() && ret == true)
	{
		if ((*item)->IsEnabled())
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
		item++;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		if ((*item)->IsEnabled())
		{
			ret = (*item)->Update(dt);
		}
		item++;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		if ((*item)->IsEnabled())
		{
			ret = (*item)->PostUpdate(dt);
		}
		item++;
	}

	FinishUpdate();

	return ret;
}
bool Application::CleanUp()
{
	bool ret = true;
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
