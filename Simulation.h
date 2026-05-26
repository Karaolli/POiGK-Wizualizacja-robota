#pragma once

#include "Manipulator.h"
#include "Scene.h"
#include "Input.h"

#include <vector>

class Simulation {
public:
	Simulation(Scene& scene);
	void Update(const Input& input, float deltaTime);
private:
	Scene& scene;
	Manipulator	manipulator;

	size_t base;
	size_t upperArm;
	size_t forearm;
	size_t wrist;
};

