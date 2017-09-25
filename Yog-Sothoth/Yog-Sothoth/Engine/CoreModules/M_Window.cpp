#include "../../Globals.h"
#include "../../Application.h"
#include "M_Window.h"
#include "../../Tools/Static/JsonSerializer.h"


M_Window::M_Window(bool enabled) : Module(enabled), window(nullptr), screen_surface(nullptr)
{
	name.assign("window");
}


M_Window::~M_Window()
{
}

bool M_Window::Init()
{
	LoadConfig();


	SDL_Log("Init SDL window & surface");
	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_Log("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 3.3
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

		if (config.fullscreen)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}
		if (config.resizable)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}
		if (config.borderless)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}
		if (config.fullscreenDesktop)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (config.maximized)
		{
			flags |= SDL_WINDOW_MAXIMIZED;
		}

		window = SDL_CreateWindow("Yog-Sothoth", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, config.w_res, config.h_res, flags);

		if (window == nullptr)
		{
			SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;

}

bool M_Window::Start()
{
	return true;
}

update_status M_Window::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status M_Window::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status M_Window::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool M_Window::CleanUp()
{
	return true;
}

void M_Window::Serialize(Json::Value& root)
{
}

void M_Window::Deserialize(Json::Value& root)
{
	config.fullscreen = root.get("fullscreen", false).asBool();
	config.borderless = root.get("borderless", false).asBool();
	config.fullscreenDesktop = root.get("fullscreen_desktop", false).asBool();
	config.maximized = root.get("maximized", false).asBool();
	config.resizable = root.get("resizable", true).asBool();
	Json::Value res = root["resolution"];
	config.w_res = res.get("w", 800).asInt();
	config.h_res = res.get("h", 600).asInt();
	config.title = root.get("title", "TITTLE_ERROR").asString();
}

void M_Window::LoadConfig()
{
	JsonSerializer::DeserializeFormPath(this, App->configPath[name]);
}

void M_Window::SaveConfig()
{
	std::string output;
	JsonSerializer::Serialize(this, output, "config/window.json");
	SDL_Log("%s", output);
}

void M_Window::setWidth(int width)
{
	SDL_SetWindowSize(window, width, config.h_res);
}

void M_Window::setHeigth(int heigth)
{
	SDL_SetWindowSize(window, config.w_res, heigth);
}

void M_Window::setBorderless(bool border)
{
	SDL_SetWindowBordered(window, (SDL_bool)border);
}

void M_Window::setFullScreen(bool fullS)
{
	if (fullS)
	{
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	}
	else
	{
		SDL_SetWindowFullscreen(window, 0);
	}
}

void M_Window::setFullScreenDesktop(bool fullSD)
{
	if(fullSD)
	{
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else
	{
		SDL_SetWindowFullscreen(window, 0);
	}
}