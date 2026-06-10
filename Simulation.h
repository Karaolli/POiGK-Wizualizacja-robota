#pragma once

#include "Manipulator.h"
#include "Scene.h"
#include "Input.h"

#include <vector>

struct OBB {
	dx::XMFLOAT3 center;
	dx::XMFLOAT3 axes[3];
	dx::XMFLOAT3 halfExtents;
};

class Simulation {
public:
	Simulation(Scene& scene);
	void Update(const Input& input, float deltaTime);

	CameraAttached camera;
	Manipulator	manipulator;
	bool IsColliding() const { return collisionDetected; }
private:
	void Grab();

	bool DetectCollision(size_t objectAindex, size_t objectBindex);
	OBB BuildOBB(size_t objectIndex);

	Scene& scene;

	bool collisionDetected = false;

	size_t grabbed = -1;

	size_t base;
	size_t upperArm;
	size_t forearm;
	size_t wrist;
	size_t primitives[4];
};