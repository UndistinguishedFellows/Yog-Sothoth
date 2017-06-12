#include "../../Globals.h"
#include "../../Application.h"
#include "M_Window.h"



M_Window::M_Window(bool enabled) : Module(enabled), window(nullptr), screen_surface(nullptr)
{
	name.assign("window");
}


M_Window::~M_Window()
{
}

bool M_Window::Init()
{
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
		int width = 1024;
		int height =720;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 3.3
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		//if (WIN_FULLSCREEN == true)
		//{
		//	flags |= SDL_WINDOW_FULLSCREEN;
		//}

		//if (WIN_RESIZABLE == true)
		//{
		//	flags |= SDL_WINDOW_RESIZABLE;
		//}

		//if (WIN_BORDERLESS == true)
		//{
		//	flags |= SDL_WINDOW_BORDERLESS;
		//}

		//if (WIN_FULLSCREEN_DESKTOP == true)
		//{
		//	flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		//}
		//if (WIN_MAXIMIZED)
		//{
		//	flags |= SDL_WINDOW_MAXIMIZED;
		//}

		window = SDL_CreateWindow("Yog-Sothoth", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if (window == NULL)
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
