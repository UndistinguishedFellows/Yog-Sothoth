#pragma once
#include "BaseClass/Module.h"
#include <SDL.h>

struct Config
{
	bool fullscreen;
	bool borderless;
	bool fullscreenDesktop;
	bool maximized;
	bool resizable;
	int w_res;
	int h_res;
	std::string title;	
};

class M_Window : public Module
{
public:

	Config config;
	Json::Value root;

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

	SDL_Window* GetWindow() const { return window; }

	void Serialize(Json::Value& root) override;
	void Deserialize(Json::Value& root) override;
	void LoadConfig() override;
	void SaveConfig() override;

	int getWidth();
	int getHeigth();
	void setWidth(int width);
	void setHeigth(int heigth);
	void setBorderless(bool border);
	void setFullScreen(bool fullS);
	void setFullScreenDesktop(bool fullSD);
	float getBrightness();
	void setBrightness(float brightness);
};

