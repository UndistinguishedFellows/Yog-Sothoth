#include "../Globals.h"
#include "../Application.h"
#include "../Engine/UI_Modules/M_UIManager.h"

void log(const char file[], int line, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	char filename[50];
	int j = 0;
	for (int i = strlen(file)-1; i > 0; --i)
	{
		if (file[i] == '/' || file[i] == '\\')
		{
			i = 0;
			filename[j] = '\0';
		}
		else
		{
			filename[j] = file[i];			
		}
		++j;
	}
	//SWAP
	for (int i = 0; i < (j-1)/2; ++i) 
	{
		char a;
		char b;
		a = filename[i];
		b = filename[strlen(filename) - 1 - i];
		filename[i] = b;
		filename[strlen(filename) - 1 - i] = a;
	}


	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", filename, line, tmp_string);
	SDL_Log(tmp_string2);
	if (App != nullptr && App->uiManager != nullptr && App->uiManager->console != nullptr)
	{
		App->uiManager->console->AddLog(tmp_string2);
	}
	

	//if (App != NULL)
	//{
	//	sprintf_s(tmp_string2, 4096, "\n%s", tmp_string);
	//	App->Log(tmp_string2);
	//}
}

void log(ConsoleType type, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	switch (type)
	{
		case CONSOLE_WARNING:
			sprintf_s(tmp_string2, 4096, "[WARNING] - %s", tmp_string);
			break;
		case CONSOLE_ERROR:
			sprintf_s(tmp_string2, 4096, "[ERROR]   - %s", tmp_string);
			break;
		case CONSOLE_MESSAGE:
			sprintf_s(tmp_string2, 4096, "%s", tmp_string);
			break;
		case CONSOLE_INFO:
			sprintf_s(tmp_string2, 4096, "[INFO]    - %s", tmp_string);
			break;
		default:
			break;
	}
	//sprintf_s(tmp_string2, 4096, "%s", tmp_string);
	SDL_Log(tmp_string2);
	if (App != nullptr && App->uiManager != nullptr && App->uiManager->console != nullptr)
	{
		App->uiManager->console->AddLog(tmp_string2);
	}


	//if (App != NULL)
	//{
	//	sprintf_s(tmp_string2, 4096, "\n%s", tmp_string);
	//	App->Log(tmp_string2);
	//}

}