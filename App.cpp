#include "App.h"

App::App()
    : window(1280, 720, L"Wizualizacja robota 3000")
    , gfx(window.GetHWND())
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

    camera.Update(input);
	sim.Update(input, deltaTime);

    input.Flush();
}
void App::Render(UINT vsync)
{
    gfx.BeginFrame();
    gfx.ClearColor({ 0.05f, 0.05f, 0.2f, 1.0f });

	gfx.ClearDepth();
    scene.Draw(gfx, camera);

	gfx.ClearDepth();
    scene.Draw(gfx, sim.camera);

    gfx.EndFrame(vsync);
}