#include "UITests.h"
#include "../../../Application.h"
#include "../../Test/YogTest.h"

UITests::UITests()
{
	active = false;
	yogTest = new YogTest();
}

UITests::~UITests()
{
}

void UITests::Draw()
{

	ImGuiWindowFlags testsWindowFlags = 0;

	ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiSetCond_FirstUseEver);
	if (!ImGui::Begin("Tests Window", &active, testsWindowFlags))
	{
		ImGui::End();
		return;
	}
	ImGui::Spacing();

	//ImGui::Text("Some content");

	if (ImGui::CollapsingHeader("MathGeoLib"))
	{
		//bool i = yogTest->SpheresIntertionTest();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "2 Spheres intersection");
		ImGui::Spacing();

		static float3 s1Pos = float3(0.0f, 0.0f, 0.0f), s2Pos = float3(0.0f, 0.0f, 0.0f);
		static float s1Rad = 0, s2Rad = 0;

		ImGui::PushItemWidth(80);

		ImGui::Text("Sphere1:"); ImGui::SameLine();
		ImGui::SliderFloat("X##s1", &s1Pos.x, -100.0f, 100.0f); ImGui::SameLine();
		ImGui::SliderFloat("Y##s1", &s1Pos.y, -100.0f, 100.0f); ImGui::SameLine();
		ImGui::SliderFloat("Z##s1", &s1Pos.z, -100.0f, 100.0f); ImGui::SameLine();
		ImGui::SliderFloat("Radius##s1", &s1Rad, 0.0f, 50.0f);

		ImGui::Text("Sphere2:"); ImGui::SameLine();
		ImGui::SliderFloat("X##s2", &s2Pos.x, -100.0f, 100.0f); ImGui::SameLine();
		ImGui::SliderFloat("Y##s2", &s2Pos.y, -100.0f, 100.0f); ImGui::SameLine();
		ImGui::SliderFloat("Z##s2", &s2Pos.z, -100.0f, 100.0f); ImGui::SameLine();
		ImGui::SliderFloat("Radius##s2", &s2Rad, 0.0f, 50.0f);

		ImGui::PopItemWidth();

		static bool checkButton = false, result = false;
		if (ImGui::Button("Intersect")) { result = yogTest->SpheresIntertionTest(s1Pos,s1Rad,s2Pos,s2Rad); checkButton = true; }
		if (checkButton)
		{
			ImGui::SameLine();
			if (result)
			{
				ImGui::TextColored(ImVec4(0.0f, 0.5f, 0.0f, 1.0f), "Yes");
			}
			else
			{
				ImGui::TextColored(ImVec4(0.5f, 0.0f, 0.0f, 1.0f), "No");
			}
			
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
	}

	ImGui::End();

}