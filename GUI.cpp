#include "GUI.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

GUI::GUI(const Window& window, const Graphics& gfx)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplWin32_Init(window.GetHWND());
    ImGui_ImplDX11_Init(gfx.GetDevice(), gfx.GetContext());
}

void GUI::Draw(Simulation& sim, float deltaTime)
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(100, 10), ImGuiCond_Once);

    ImGui::Begin("Control");

	auto label = [](const char* text) {
		ImGui::SetNextItemWidth(150.0f);
		ImGui::LabelText("##label", text);
		ImGui::SameLine();
		};

    label("Angle 1 [Rad]");
    label("Angle 2 [Rad]");
    label("Angle 3 [Rad]");
    label("Angle 4 [Rad]");
    label("Angle 5 [Rad]");
    label("Angle 6 [Rad]");

    float baseYawTarget       = sim.manipulator.GetBaseYaw();
    float shoulderPitchTarget = sim.manipulator.GetShoulderPitch();
    float elbowPitchTarget    = sim.manipulator.GetElbowPitch();
    float wristPitchTarget    = sim.manipulator.GetWristPitch();
    float wristYawTarget      = sim.manipulator.GetWristYaw();
    float wristRollTarget     = sim.manipulator.GetWristRoll();

    auto slider = [&](const char* id, float& value, float min, float max) {
        ImGui::SetNextItemWidth(150.0f);
        if (ImGui::SliderFloat(id, &value, min, max)) {
            sim.manipulator.baseYawTarget       = baseYawTarget;
            sim.manipulator.shoulderPitchTarget = shoulderPitchTarget;
            sim.manipulator.elbowPitchTarget    = elbowPitchTarget;
            sim.manipulator.wristPitchTarget    = wristPitchTarget;
            sim.manipulator.wristYawTarget      = wristYawTarget;
            sim.manipulator.wristRollTarget     = wristRollTarget;
            sim.manipulator.moving = true;
        }
        ImGui::SameLine();
        };

    ImGui::NewLine();
    slider("##1slider", baseYawTarget      , sim.manipulator.GetBaseMinYaw()      , sim.manipulator.GetBaseMaxYaw());
    slider("##2slider", shoulderPitchTarget, sim.manipulator.GetShoulderMinPitch(), sim.manipulator.GetShoulderMaxPitch());
    slider("##3slider", elbowPitchTarget   , sim.manipulator.GetElbowMinPitch()   , sim.manipulator.GetElbowMaxPitch());
    slider("##4slider", wristPitchTarget   , sim.manipulator.GetWristMinPitch()   , sim.manipulator.GetWristMaxPitch());
    slider("##5slider", wristYawTarget     , sim.manipulator.GetWristMinYaw()     , sim.manipulator.GetWristMaxYaw());
    slider("##6slider", wristRollTarget    , sim.manipulator.GetWristMinRoll()    , sim.manipulator.GetWristMaxRoll());

    static float xTarget = 0.0f;
    static float yTarget = 0.0f;
    static float zTarget = 0.0f;

    ImGui::NewLine();
	label("Target X");
	label("Target Y");
	label("Target Z");

    ImGui::NewLine();
    slider("##x", xTarget, -5.0f, 5.0f);
    slider("##y", yTarget, -5.0f, 5.0f);
    slider("##z", zTarget, -5.0f, 5.0f);

    static const char* moveText = "";

	if (ImGui::Button("Move to target")) {
        if (!sim.manipulator.SetTarget(xTarget, yTarget, zTarget, wristPitchTarget, wristYawTarget, wristRollTarget))
            moveText = "Impossible move.";
        else
            moveText = "";
	}

	ImGui::SameLine();
    ImGui::SetNextItemWidth(200.0f);
	ImGui::LabelText("##label", moveText);

    static const char* collisionText = "";

    if (sim.IsColliding())
        collisionText = "Collision detected.";
    else
        collisionText = "No collision.";

    ImGui::SameLine();
    ImGui::SetNextItemWidth(150.0f);
    ImGui::LabelText("##label", collisionText);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void GUI::Shutdown()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
