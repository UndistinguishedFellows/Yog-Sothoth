#include <iostream>
#include <SDL.h>
#include "Application.h"

#include "Tools/Timer.h"

enum main_states
{
	MAIN_CREATION,
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};


Application* App = nullptr;

int main(int argc, char** argv)
{
	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;

	Timer appTimer;
	float dt;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
			case MAIN_CREATION:

				SDL_Log("-------------- Application Creation --------------");
				App = new Application();
				appTimer.Start();
				state = MAIN_START;
				break;

			case MAIN_START:

				SDL_Log("-------------- Application Init --------------");
				if (App->Init() == false)
				{
					SDL_Log("Application Init exits with ERROR");
					state = MAIN_EXIT;
				}
				else
				{
					state = MAIN_UPDATE;
					SDL_Log("-------------- Application Update --------------");
				}

				break;

			case MAIN_UPDATE:
			{
				dt = static_cast<float>(appTimer.Read() / 1000.0f);
				appTimer.Start();
				int update_return = App->Update(dt);

				if (update_return == UPDATE_ERROR)
				{
					SDL_Log("Application Update exits with ERROR");
					state = MAIN_EXIT;
				}

				if (update_return == UPDATE_STOP)
					state = MAIN_FINISH;
			}
			break;

			case MAIN_FINISH:

				SDL_Log("-------------- Application CleanUp --------------");
				if (App->CleanUp() == false)
				{
					SDL_Log("Application CleanUp exits with ERROR");
				}
				else
					main_return = EXIT_SUCCESS;

				state = MAIN_EXIT;

				break;
			default: 
				SDL_Log("Application MainLoop exits with ERROR");
				main_return = EXIT_FAILURE;
		}
	}

	delete App;
	return main_return;

}