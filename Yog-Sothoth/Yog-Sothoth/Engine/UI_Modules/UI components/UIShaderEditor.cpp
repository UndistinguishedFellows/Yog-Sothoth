#include "UIShaderEditor.h"
#include "../../../Application.h"
#include <filesystem>

namespace fs = std::experimental::filesystem;

UIShaderEditor::UIShaderEditor()
{
	active = false;
	fileOpen = false;

	path = "data/shaders/";

	///////////////////////////////////////////////////////////////////////
	// TEXT EDITOR INIT

	// Select language
	auto lang = TextEditor::LanguageDefinition::GLSL();

	// Preprocessor symbols
	static const char* ppnames[] = { "NULL" };

	static const char* ppvalues[] = { "#define NULL ((void*)0)" };

	for (int i = 0; i < sizeof(ppnames) / sizeof(ppnames[0]); ++i)
	{
		TextEditor::Identifier id;
		id.mDeclaration = ppvalues[i];
		lang.mPreprocIdentifiers.insert(std::make_pair(std::string(ppnames[i]), id));
	}

	// Identifiers
	static const char* identifiers[] = {
		"UINT", "UINT32", "UINT64", "UCHAR", };

	static const char* idecls[] =
	{
		"typedef unsigned int uint", "typedef unsigned __int32 uint32", "typedef unsigned __int64 uint64", "typedef unsigned char uchar", };

	for (int i = 0; i < sizeof(identifiers) / sizeof(identifiers[0]); ++i)
	{
		TextEditor::Identifier id;
		id.mDeclaration = std::string(idecls[i]);
		lang.mIdentifiers.insert(std::make_pair(std::string(identifiers[i]), id));
	}

	editor.SetLanguageDefinition(lang);

	// Error markers
	//TextEditor::ErrorMarkers markers;
	//Error example:
	//	markers.insert(std::make_pair<int, std::string>(14, "Example error here:\nInclude file not found: \"TextEditor.h\""));
	//editor.SetErrorMarkers(markers);

	// "Breakpoint" markers
	//TextEditor::Breakpoints bpts;
	//BreakPoinr Example:
	//	bpts.insert(24);
	//editor.SetBreakpoints(bpts);

	//
	//////////////////////////////////////////////////////////////////////
}

UIShaderEditor::~UIShaderEditor()
{
}

void UIShaderEditor::Draw()
{
	///////////////////////////////////////////////////////////////////////
	// TEXT EDITOR WINDOW

	//ImGUI
	auto cpos = editor.GetCursorPosition();

	ImGuiWindowFlags shaderEditorWindowFlags = 0;
	shaderEditorWindowFlags |= ImGuiWindowFlags_HorizontalScrollbar;
	shaderEditorWindowFlags |= ImGuiWindowFlags_MenuBar;

	ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiSetCond_FirstUseEver);
	if (!ImGui::Begin("Shader Editor", &active, shaderEditorWindowFlags))
	{
		ImGui::End();
		return;
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImColor color(66, 66, 66);
			ImColor colorHover(92, 92, 92);
			ImGui::PushStyleColor(ImGuiCol_Button, color);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colorHover);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, colorHover);

			if (ImGui::Button("Open..."))
			{
				ImGui::OpenPopup("ShadersList");
			}
			if (ImGui::BeginPopup("ShadersList"))
			{
				ImGui::MenuItem("Shaders", NULL, false, false);
				ImGui::Separator();

				for (auto p : fs::directory_iterator(path.c_str()))
				{
					fs::path file(p);
					std::string name(file.stem().string());
					std::string ext(file.extension().string());
					std::string completePath("");
					std::string nameExtMenu("");

					if (strcmp(ext.c_str(), ".fs") == 0 || strcmp(ext.c_str(), ".vs") == 0 || strcmp(ext.c_str(), ".gs") == 0)
					{
						nameExtMenu.append(name);
						nameExtMenu.append(ext);
						if (ImGui::MenuItem(nameExtMenu.c_str()))
						{
							completePath.append(path);
							completePath.append(name);
							completePath.append(ext);
							
							nameExt = "";
							nameExt.append(name);
							nameExt.append(ext);

							fileOpen = true;

							//File to edit
							std::ifstream t(completePath);
							std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
							editor.SetText(str);

							//App->objManager->LoadScenePrefab(completePath);
						}
					}
				}

				ImGui::EndPopup();
			}

			if (ImGui::MenuItem("Save",NULL,false,fileOpen))
			{
				auto t = editor.GetText();
				std::string savePath("");

				savePath.append(path);
				savePath.append(nameExt);

				std::ofstream file(savePath, std::ofstream::out);

				file.write(t.c_str(), t.size());
			}

			ImGui::PopStyleColor(3);

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			bool ro = editor.IsReadOnly();
			if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
				editor.SetReadOnly(ro);
			ImGui::Separator();

			if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && editor.CanUndo()))
				editor.Undo();
			if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor.CanRedo()))
				editor.Redo();

			ImGui::Separator();

			if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor.HasSelection()))
				editor.Copy();
			if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor.HasSelection()))
				editor.Cut();
			if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor.HasSelection()))
				editor.Delete();
			if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
				editor.Paste();

			ImGui::Separator();

			if (ImGui::MenuItem("Select all", nullptr, nullptr))
				editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(editor.GetTotalLines(), 0));

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Dark palette"))
				editor.SetPalette(TextEditor::GetDarkPalette());
			if (ImGui::MenuItem("Light palette"))
				editor.SetPalette(TextEditor::GetLightPalette());
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
		editor.IsOverwrite() ? "Ovr" : "Ins",
		editor.CanUndo() ? "*" : " ",
		editor.GetLanguageDefinition().mName.c_str(), nameExt.c_str());

	editor.Render("TextEditor");
	ImGui::End();

	//
	//////////////////////////////////////////////////////////////////////
}