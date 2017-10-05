#pragma once
#include "BaseClass/UIComponent.h"
#include "../../../imGUI/imgui.h"

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

	ImGuiWindowFlags inspectorWindowFlags = 0;
};
