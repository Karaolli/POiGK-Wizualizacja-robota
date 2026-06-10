#pragma once

#include "Window.h"
#include "Graphics.h"
#include "Simulation.h"

class GUI
{
public:
	GUI(const Window& window, const Graphics& gfx);
	void Draw(Simulation& sim, float deltaTime);
	void Shutdown();
};