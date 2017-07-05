#include "ConsoleCommands.h"
#include "../../Application.h"


void ConsoleCommands::EnterCommand(int argc, char** args)
{
	if (argc <= 0)
		return;

	if (std::strcmp(args[0], "window") == 0)
	{
		if (argc > 1)
		{
			for (int i = 1; i < argc; ++i)
			{
				if (std::strcmp(args[1], "-help") == 0 && argc == 2)
				{
					yogConsole("Window Comands:");
					yogConsole("-info to get info about window");
					yogConsole("-setTitle [newTitle] to set the new tittle");

				}
				else if (std::strcmp(args[1], "-help") == 0 && argc == 3)
				{
					if (std::strcmp(args[2], "-info"))
					{
						yogConsole("Prints size and basic properties of window");
					}
					else if (std::strcmp(args[2], "-setTitle"))
					{
						yogConsole("Sets a new title for the window, overriding the config.json");
					}
				}
				else if (std::strcmp(args[1], "-info") == 0 && argc == 2)
				{
					yogConsole("%s", App->window->AsString().c_str());
				}
			}
		}
		else
		{
			yogConsole("You should enter some params to this command:");
			yogConsole("(window -help) for more info");
			yogConsole("(window -help param) for more info about the parameter");
		}
	}
}
