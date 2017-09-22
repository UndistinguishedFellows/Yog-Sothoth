﻿#pragma once
#include "BaseClass/UIComponent.h"

class UIWindowMenus : UIComponent
{
public:
	UIWindowMenus();
	~UIWindowMenus();

	void Draw() override;
	void ShowCredits();

	bool quitSelected = false;
	bool creditsSelected = false;
	bool openReleaseDirectory = false;
	bool openRepoDirectory = false;
	bool openIssuesDirectory = false;
	bool showfpsInfo = false;

	bool QUIT = false;


	bool credits = false;

};
