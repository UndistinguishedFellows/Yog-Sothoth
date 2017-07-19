#pragma once
#include "BaseClass\UIComponent.h"
#include "../../imGUI/imgui.h"
#include <ctype.h>
#include <istream>


#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

class UIConsole : public UIComponent
{
public:
	char                  InputBuf[256];
	ImVector<char*>       Items;
	bool                  ScrollToBottom;
	ImVector<char*>       History;
	int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
	ImVector<const char*> Commands;

	bool pOpen = false;


public:
	UIConsole();
	~UIConsole();

	void Draw() override;
	void Draw(const char* title, bool* p_open);

	static int   Stricmp(const char* str1, const char* str2) { int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
	static int   Strnicmp(const char* str1, const char* str2, int n) { int d = 0; while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; n--; } return d; }
	static char* Strdup(const char *str) { size_t len = strlen(str) + 1; void* buff = malloc(len); return (char*)memcpy(buff, (const void*)str, len); }

	void ClearLog();
	void AddLog(const char* fmt, ...);
	void ExecCommand(const char* command_line);

	static int TextEditCallbackStub(ImGuiTextEditCallbackData* data) // In C++11 you are better off using lambdas for this sort of forwarding callbacks
	{
		UIConsole* console = (UIConsole*)data->UserData;
		return console->TextEditCallback(data);
	}
	int TextEditCallback(ImGuiTextEditCallbackData* data);


};

