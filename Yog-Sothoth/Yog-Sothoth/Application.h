#pragma once

#include "Globals.h"
#include <list>
#include "Engine/CoreModules/M_Window.h"
#include "Engine/CoreModules/M_Renderer.h"
#include "Engine/UI_Modules/M_UIManager.h"
#include "Engine/CoreModules/M_Input.h"
#include "Engine/CoreModules/M_FileSystem.h"
#include <GameConsole.h>
#include "Tools/SimpleYogLog.h"
#include "Engine/GameObjects/M_ObjectManager.h"
#include "Tools/YogClock.h"
#include "Engine/CoreModules/M_ResourceManager.h"

class Module;
class M_FileSystem;

using namespace std::placeholders;

struct HardwareInfo
{
	const char* platform;//SDL_GetPlatform(void)
	SDL_version sdlVersion;
	int numCPUs;
	int cache;
	int systemRAM;
	bool DNow;
	bool AVX;
	bool AVX2;
	bool altiVec;
	bool MMX;
	bool RDTSC;
	bool SSE;
	bool SSE2;
	bool SSE3;
	bool SSE41;
	bool SSE42;
};

class Application : public IJsonSerializable
{
public:
	bool quit;
	/**
	 * \brief A map to store config path of all modules
	 * Key: module name
	 * Value: path
	 */
	std::map<std::string, std::string> configPath;
	M_Window* window = nullptr;
	M_Renderer* renderer = nullptr;
	M_UIManager* uiManager = nullptr;
	M_Input* input = nullptr;
	M_FileSystem* fs = nullptr;
	M_ResourceManager* resourceManager = nullptr;
	M_ObjectManager* objManager = nullptr;

	std::string organization;
	std::string appName;

	//Console
	std::istringstream* inputStream = nullptr;
	std::ostringstream* outputStream = nullptr;

	//SimpleYogLog log;
	//Virtuoso::GameConsole console;

	YogClock appTimer;
	game_status gameStatus = game_status::STOP;
	HardwareInfo hardwareInfo;

	float MaxFPS;

private:
	std::list<Module*> list_modules;

public:
	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	bool OpenBrowser(const char* link);
	void Log(char* str);

	char* getOrganization();
	char* getTitle();

	void addModule(Module* module);
	void removeModule(Module* module);

	void Serialize(Json::Value& root) override;
	void Deserialize(Json::Value& root) override;
	void LoadConfig();
	void SaveConfig();
private:
	void PrepareUpdate();
	void FinishUpdate();
	void loadHardwareInfo();

	void updateGameStatus();
};

extern Application* App;