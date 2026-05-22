#include "App.h"

App::App()
    : window(1280, 720, L"Wizualizacja robota 3000")
    , gfx(window.GetHWND(), window.GetWidth(), window.GetHeight())
    , scene(gfx)
{
    window.app = this;
}

int App::Run() {
    Transform cubeTransform  = { { 1, 0,  3 }, { 0, 0,   0, 1 }, { 1, 1, 1 } };
    Transform cubeTransform2 = { { 0, 0,  0 }, { 0, 1, 0.5, 1 }, { 1, 1, 1 } };
    Transform cubeTransform3 = { { 0, 0, -1 }, { 0, 0, 0.5, 1 }, { 1, 5, 1 } };
    Transform cubeTransform4 = { { 0, 0, -6 }, { 0, 0, 0.5, 1 }, { 2, 2, 1 } };

    scene.AddObject(scene.cubeMesh, cubeTransform);
    scene.AddObject(scene.cubeMesh, cubeTransform2);
    scene.AddObject(scene.cubeMesh, cubeTransform3);
    scene.AddObject(scene.cubeMesh, cubeTransform4);

    Transform cubeTransform5 = { scene.lightCB.data.position, { 0, 0, 0, 1 }, { 0.1, 0.1, 0.1 } };
    scene.AddObject(scene.cubeMesh, cubeTransform5);

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

    float right = (input.keys['D'] ? 1.0f : 0.0f) - (input.keys['A'] ? 1.0f : 0.0f);
	float up    = (input.keys['E'] ? 1.0f : 0.0f) - (input.keys['Q'] ? 1.0f : 0.0f);
    float fwd   = (input.keys['W'] ? 1.0f : 0.0f) - (input.keys['S'] ? 1.0f : 0.0f);

    scene.lightCB.data.position.x += right * 1.0f * deltaTime;
    scene.lightCB.data.position.y += up    * 1.0f * deltaTime;
    scene.lightCB.data.position.z += fwd   * 1.0f * deltaTime;

	scene.objects[4].transform.position = scene.lightCB.data.position;
}

void App::Render(UINT vsync)
{
    gfx.BeginFrame();
    scene.Draw(gfx, window.GetAspectRatio());
    gfx.EndFrame(vsync);
}