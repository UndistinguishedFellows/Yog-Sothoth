#pragma once

#include "../../../imGUI/imgui.h"
#include "BaseClass/UIComponent.h"

#include "../../ImGuiColorTextEdit/TextEditor.h"
#include <string>

class GameObject;

class UIShaderEditor : public UIComponent
{
public:
	UIShaderEditor();
	~UIShaderEditor();

	void Draw() override;

	ImGuiWindowFlags shaderEditorWindowFlags = 0;

	TextEditor editor;
	std::string path;
	std::string nameExt;
	bool fileOpen;
};