#include "Simulation.h"

Simulation::Simulation(Scene& scene) : scene(scene)
{
	Transform baseTransform = { {}, {}, {1, manipulator.GetBaseHeight(), 1}};
	baseTransform.SetPositionRotationMatrix(manipulator.GetBaseTransform());
	base = scene.AddObject(scene.cubeMesh, baseTransform);
	
	Transform shoulderTransform = { {}, {}, {0.4, manipulator.GetUpperArmLength(), 0.2} };
	shoulderTransform.SetPositionRotationMatrix(manipulator.GetShoulderTransform());
	upperArm = scene.AddObject(scene.cubeMesh, shoulderTransform);

	Transform elbowTransform = { {}, {}, {0.3, manipulator.GetForearmLength(), 0.15} };
	elbowTransform.SetPositionRotationMatrix(manipulator.GetElbowTransform());
	forearm = scene.AddObject(scene.cubeMesh, elbowTransform);

	Transform wristTransform = { {}, {}, {0.4, manipulator.GetWristLength(), 0.4} };
	wristTransform.SetPositionRotationMatrix(manipulator.GetWristTransform());
	wrist = scene.AddObject(scene.cubeMesh, wristTransform);
}

void Simulation::Update(const Input& input, float deltaTime)
{
	float baseControl     = (input.keys['D'] ? 1.0f : 0.0f) - (input.keys['A'] ? 1.0f : 0.0f);
	float shoulderControl = (input.keys[VK_CONTROL] ? 1.0f : 0.0f) - (input.keys[' '] ? 1.0f : 0.0f);
	float elbowControl    = (input.keys['W'] ? 1.0f : 0.0f) - (input.keys['S'] ? 1.0f : 0.0f);

	float wristPitchControl = (input.keys['I'] ? 1.0f : 0.0f) - (input.keys['K'] ? 1.0f : 0.0f);
	float wristYawControl   = (input.keys['J'] ? 1.0f : 0.0f) - (input.keys['L'] ? 1.0f : 0.0f);
	float wristRollControl  = (input.keys['U'] ? 1.0f : 0.0f) - (input.keys['O'] ? 1.0f : 0.0f);

	float speedMultiplier = (input.keys['C'] ? 1.5f : 1.0f) * (input.keys['X'] ? 0.5f : 1.0f);

	manipulator.RotateJoint(0, baseControl     * deltaTime * speedMultiplier);
	manipulator.RotateJoint(1, shoulderControl * deltaTime * speedMultiplier);
	manipulator.RotateJoint(2, elbowControl    * deltaTime * speedMultiplier);
	manipulator.RotateJoint(3, wristPitchControl * deltaTime * speedMultiplier);
	manipulator.RotateJoint(4, wristYawControl   * deltaTime * speedMultiplier);
	manipulator.RotateJoint(5, wristRollControl  * deltaTime * speedMultiplier);

	scene.objects[base    ].transform.SetPositionRotationMatrix(manipulator.GetBaseTransform());
	scene.objects[upperArm].transform.SetPositionRotationMatrix(manipulator.GetShoulderTransform());
	scene.objects[forearm ].transform.SetPositionRotationMatrix(manipulator.GetElbowTransform());
	scene.objects[wrist   ].transform.SetPositionRotationMatrix(manipulator.GetWristTransform());

	camera.Update(manipulator.GetWristTransform());
	camera.SetFOV(90.0f);
}