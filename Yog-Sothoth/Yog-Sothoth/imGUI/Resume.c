// This is not to compile

//Float draggable
ImGui::DragFloat3("drag float3", vec4f, 0.01f, 0.0f, 1.0f);

//Overlay to count fps and show simple info
static void ShowExampleAppFixedOverlay(bool* p_open)
{
    ImGui::SetNextWindowPos(ImVec2(10,10));
    if (!ImGui::Begin("Example: Fixed Overlay", p_open, ImVec2(0,0), 0.3f, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::End();
        return;
    }
    ImGui::Text("Simple overlay\non the top-left side of the screen.");
    ImGui::Separator();
    ImGui::Text("Mouse Position: (%.1f,%.1f)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
    ImGui::End();
}


//Inspector
    //Window
    ImGui::SetNextWindowPos(ImVec2(100,100), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Same title as another window##1");
    ImGui::Text("This is window 1.\nMy title is the same as window 2, but my identifier is unique.");
    ImGui::End();

    //Float Position
    ImGui::DragFloat3("drag float3", vec4f, 0.01f, 0.0f, 1.0f);
    //Float Scale
    ImGui::DragFloat3("drag float3", vec4f, 0.01f, 0.0f, 1.0f);
    //Float Rotation
    ImGui::DragFloat3("drag float3", vec4f, 0.01f, 0.0f, 1.0f);