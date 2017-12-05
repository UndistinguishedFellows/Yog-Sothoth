#pragma once
#include "../../Globals.h"
#include "../CoreModules/BaseClass/Module.h"
#include "UI components\UIConsole.h"
#include "UI components\UIFrameCounter.h"
#include "UI components/UIOutliner.h"
#include "UI components/UItests.h"
#include "UI components/UIConfiguration.h"
#include "UI components/UIWindowMenus.h"
#include "UI components/UIInspector.h"
#include "UI components/UIGizmos.h"
#include "UI components/UITimeControllers.h"
#include "UI components/UIWaterShader.h"

class M_UIManager : public Module
{
public:
	UIConsole* console = nullptr;
	UIFrameCounter* frameCounter = nullptr;
	UIOutliner* outliner = nullptr;
	UITests* tests = nullptr;
	UIConfiguration* configuration = nullptr;
	UIWindowMenus* menus = nullptr;
	UIInspector* inspector = nullptr;
	UIGizmos *gizmos = nullptr;
	UITimeControllers *timeControllers = nullptr;
	UIWaterShader * water_shader = nullptr;

	bool outlinerSelected = false;
	bool isUsingMouse;
	bool isUsingKeyboard;

public:
	M_UIManager(bool enabled = true);
	~M_UIManager();

	bool Init() override;
	bool Start() override;
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	update_status PostUpdate(float dt) override;
	void DrawEditor();
	bool CleanUp() override;

	void Serialize(Json::Value& root) override;
	void Deserialize(Json::Value& root) override;
	void LoadConfig() override;
	void SaveConfig() override;
	
	void setGreyStyle();
};
