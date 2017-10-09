#include "../../Application.h"
#include "M_Input.h"
#include "M_Renderer.h"

#include "../../imGUI/imgui.h"
#include "../../imGUI/imgui_impl_sdl_gl3.h"
#include "../GameObjects/Components/C_Camera.h"
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

#define MAX_KEYS 300

M_Input::M_Input(bool enable) : Module(enable)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
M_Input::~M_Input()
{
	delete[] keyboard;
}

// Called before render is available
bool M_Input::Init()
{
	SDL_Log("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	//SDL_ShowCursor(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		SDL_Log("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status M_Input::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= 1024; //FIX: Put dinamig assignment #resolution
	mouse_y /= 720;
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;


	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		ImGui_ImplSdlGL3_ProcessEvent(&e);
		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
				mouse_x = e.motion.x;// / 1024; //FIX: Put dinamic assignment #resolution
				mouse_y = e.motion.y;// / 720;

			mouse_x_motion = e.motion.xrel;// / 1024;//FIX: Put dinamic assignment #resolution
				mouse_y_motion = e.motion.yrel;// / 720;
			break;

			case SDL_QUIT:
			App->quit = true;
			break;
			case SDL_DROPFILE:
			{      // In case if dropped file
				dropped_filedir = e.drop.file;
				// Shows directory of dropped file
//				SDL_ShowSimpleMessageBox(
//					SDL_MESSAGEBOX_INFORMATION,
//					"File dropped on window",
//					dropped_filedir,
//					App->window->window
//				);
//				std::string str("data/assets/");
//				str.append(fs::path(dropped_filedir).filename().u8string());
				App->objManager->LoadFBXFromDragAndDrop(dropped_filedir);
				C_Camera* camera = (C_Camera*)App->objManager->camera->FindComponent(C_CAMERA);
				camera->LookAt(float3(0, 0, 0));
				SDL_free(dropped_filedir);    // Free dropped_filedir memory
				break;
			}
			case SDL_WINDOWEVENT:
			{
				//if(e.window.event == SDL_WINDOWEVENT_RESIZED)
				//	App->renderer->OnResize(e.window.data1, e.window.data2);
			}
		}
	}

	SDL_PollEvent(&e);

	if(App->quit == true/* || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP*/)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool M_Input::CleanUp()
{
	SDL_Log("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

void M_Input::Serialize(Json::Value& root)
{
}

void M_Input::Deserialize(Json::Value& root)
{
}

void M_Input::LoadConfig()
{
}

void M_Input::SaveConfig()
{
}
