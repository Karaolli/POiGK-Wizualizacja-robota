#pragma once

#include "Input.h"
#include "Window.h"
#include "Graphics.h"
#include "Scene.h"
#include "Simulation.h"

#include <chrono>

class App {
public:
	App();
	int  Run(); // Główna pętla
	void Update();
	void Render(UINT vsync);

	Input       input;
	Window      window;
	Graphics    gfx;
	CameraOrbit camera;
	Scene       scene;
	Simulation  sim;
private:
	std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
};