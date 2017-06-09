#pragma once
#include "BaseClass/Module.h"
#include <SDL.h>

class M_Window : public Module
{
public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;


	M_Window(bool enabled = true);
	~M_Window();

	bool Init() override;
	bool Start() override;
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	bool CleanUp() override;

	SDL_Window* GetWindow() { return window; }



};

