#include "UIShaderEditor.h"
#include "../../../Application.h"
#include <filesystem>

UIShaderEditor::UIShaderEditor()
{
	active = false;

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

	//File to edit
	std::ifstream t("data/shaders/1.basic_lighting.fs");
	std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	editor.SetText(str);

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
			if (ImGui::MenuItem("Save"))
			{
				auto t = editor.GetText();
				/// save text in t....
			}
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
		editor.GetLanguageDefinition().mName.c_str(), "$$FileName$$");

	editor.Render("TextEditor");
	ImGui::End();

	//
	//////////////////////////////////////////////////////////////////////
}