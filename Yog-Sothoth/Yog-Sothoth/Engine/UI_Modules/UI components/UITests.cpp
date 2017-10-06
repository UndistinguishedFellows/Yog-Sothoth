#include "UITests.h"
#include "../../../Application.h"
#include "../../Test/YogTest.h"
#include "../../CoreModules/M_Renderer.h"

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

	if (ImGui::CollapsingHeader("MathGeoLib"))
	{
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

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Float Number Generator (0.0-1.0)");
		ImGui::Spacing();

		static bool genRandFloatButton = false;
		static float randFloat = 0;
		
		if (ImGui::Button("Generate##float")) {
			randFloat = yogTest->getRandomFloat();
			genRandFloatButton = true;
		}
		if (genRandFloatButton)
		{
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.0f, 0.5f, 0.0f, 1.0f), "%.3f", randFloat);
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Integer Number Generator");
		ImGui::Spacing();

		static int minInt = 0, maxInt = 0;

		ImGui::PushItemWidth(160);

		ImGui::SliderInt("Min##int", &minInt, -100, 100); ImGui::SameLine();
		ImGui::SliderInt("Max##int", &maxInt, -100, 100);

		static bool genRandIntButton = false;
		static int randInt = 0;
		if (minInt > maxInt)
		{
			ImGui::TextColored(ImVec4(0.5f, 0.0f, 0.0f, 1.0f), "Min must be lower than Max.");
		}
		if (ImGui::Button("Generate##int")) {
			if (minInt < maxInt)
			{
				randInt = yogTest->getRandomInt(minInt,maxInt);
				genRandIntButton = true;
			}
		}
		if (genRandIntButton)
		{
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.0f, 0.5f, 0.0f, 1.0f), "%d",randInt);
		}
	}

	if (ImGui::CollapsingHeader("OpenGL"))
	{
		bool drawCheckersCube = App->renderer->checkersCube;
		if (ImGui::Checkbox("Draw checkers cube", &drawCheckersCube))
		{
			App->renderer->checkersCube = drawCheckersCube;
		}
	}

	ImGui::End();

}