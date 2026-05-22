#pragma once

#include "Input.h"
#include "Window.h"
#include "Graphics.h"
#include "Scene.h"

#include <chrono>

class App {
public:
	App();
	int  Run();
	void Update();
	void Render(UINT vsync);

	Input input;
	Window window;
	Graphics gfx;
	Scene scene;
private:
	std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
};