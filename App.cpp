#include "App.h"

App::App()
    : window(1280, 720, L"Wizualizacja robota 3000")
    , gfx(window.GetHWND(), window.GetWidth(), window.GetHeight())
    , scene(gfx)
	, sim(scene)
{
    window.app = this;
}

int App::Run() {
    while (window.ProcessMessages()) {
        if (window.IsMinimized()) continue;

        Update();
        Render(0u);
    }
    return 0;
}

void App::Update()
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    float deltaTime = std::chrono::duration<float>(now - lastTime).count();
    lastTime = now;

    if (input.mouseButtons[1])
        scene.camera.OnMouseDrag(input.mouseDX, input.mouseDY);
    if (input.mouseScrollDelta != 0.0f)
        scene.camera.OnMouseScroll(input.mouseScrollDelta);

    input.Flush();

	sim.Update(input, deltaTime);
}

void App::Render(UINT vsync)
{
    gfx.BeginFrame();
    scene.Draw(gfx, window.GetAspectRatio());
    gfx.EndFrame(vsync);
}