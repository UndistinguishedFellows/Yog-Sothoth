#pragma once
#include "BaseClass/UIComponent.h"
#include "../../../imGUI/imgui.h"
#include <string>

class UIInspector : public UIComponent
{
public:
	UIInspector();
	~UIInspector();

	void Draw() override;
	void Transform();
	void Mesh();
	void Material();
	void Light();
	void Camera();
	void Shader();
	void TreeNodes(std::string node);
	void ShowAddComponentMenu();

	ImGuiWindowFlags inspectorWindowFlags = 0;
};
