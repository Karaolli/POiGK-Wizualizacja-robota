#include "Simulation.h"

#include <random>


Simulation::Simulation(Scene& scene) : scene(scene)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(0.0f, 1.0f);

	Transform groundTransform = { {0, -0.5, 1}, {0, 0, 0, 1}, {2, 1, 4} };
	size_t ground = scene.AddObject(scene.cubeMesh, groundTransform);

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

	camera.SetFOV(90.0f);

	for (size_t i = 0; i < std::size(primitives); i++) { // generowanie losowo rozmieszczonych obiektów
		primitives[i] = scene.AddObject(scene.cubeMesh,
			{ {dist(gen) * 8 - 4.0f, dist(gen) * 8 - 4.0f, dist(gen) * 8 - 4.0f},
			{dist(gen), dist(gen), dist(gen), dist(gen)},
			{1, 1, 1} });
		for (size_t j = 0; j < std::size(scene.objects); j++) { // sprawdzenie kolizji
			if (j == primitives[i]) continue;
			if (DetectCollision(primitives[i], j)) {
				scene.objects[primitives[i]].transform.position = { dist(gen) * 8 - 4.0f, dist(gen) * 8 - 4.0f, dist(gen) * 8 - 4.0f };
				scene.objects[primitives[i]].transform.rotation = { dist(gen), dist(gen), dist(gen), dist(gen) };
				j = -1;
			}
		}
	}
}

void Simulation::Update(const Input& input, float deltaTime)
{
	if (manipulator.IsMoving()) { // ruch do celu
		manipulator.MoveTowardsTarget(deltaTime);
	}
	else { // ruch ręczny
		float baseControl     = (input.down['D'] ? 1.0f : 0.0f) - (input.down['A'] ? 1.0f : 0.0f);
		float shoulderControl = (input.down['C'] ? 1.0f : 0.0f) - (input.down[' '] ? 1.0f : 0.0f);
		float elbowControl    = (input.down['W'] ? 1.0f : 0.0f) - (input.down['S'] ? 1.0f : 0.0f);

		float wristPitchControl = (input.down['I'] ? 1.0f : 0.0f) - (input.down['K'] ? 1.0f : 0.0f);
		float wristYawControl   = (input.down['J'] ? 1.0f : 0.0f) - (input.down['L'] ? 1.0f : 0.0f);
		float wristRollControl  = (input.down['U'] ? 1.0f : 0.0f) - (input.down['O'] ? 1.0f : 0.0f);

		float speedMultiplier = (input.down['Z'] ? 1.5f : 1.0f) * (input.down['X'] ? 0.5f : 1.0f);

		manipulator.RotateJoint(0, baseControl     * deltaTime * speedMultiplier);
		manipulator.RotateJoint(1, shoulderControl * deltaTime * speedMultiplier);
		manipulator.RotateJoint(2, elbowControl    * deltaTime * speedMultiplier);
		manipulator.RotateJoint(3, wristPitchControl * deltaTime * speedMultiplier);
		manipulator.RotateJoint(4, wristYawControl   * deltaTime * speedMultiplier);
		manipulator.RotateJoint(5, wristRollControl  * deltaTime * speedMultiplier);
	}

	scene.objects[base    ].transform.SetPositionRotationMatrix(manipulator.GetBaseTransform());
	scene.objects[upperArm].transform.SetPositionRotationMatrix(manipulator.GetShoulderTransform());
	scene.objects[forearm ].transform.SetPositionRotationMatrix(manipulator.GetElbowTransform());
	scene.objects[wrist   ].transform.SetPositionRotationMatrix(manipulator.GetWristTransform());

	camera.Update(manipulator.GetWristTransform());

	if (input.pressed['R']) { // chwycanie
		if (grabbed == -1)
			Grab();
		else
			grabbed = -1;
	}
	if (grabbed != -1) // przemieszczanie chwyconego obiektu
		scene.objects[grabbed].transform.SetPositionRotationMatrix(
			dx::XMMatrixTranslation(0.0f, 0.8f, 0.0f) * manipulator.GetWristTransform());

	if (grabbed != -1) // wykrycie kolizji chwyconego obiektu
		for (size_t i = 0; i < std::size(scene.objects); i++) {
			if (i == grabbed) continue;
			if (DetectCollision(i, grabbed)) {
				collisionDetected = true;
				break;
			}
			else collisionDetected = false;
		}
}


void Simulation::Grab() // Znalezienie najbliższego obiektu do chwytaka i chwycanie, jeśli w zasięgu
{
	float closestDistance = FLT_MAX;
	size_t closestObject = -1;
	for (size_t i = 0; i < std::size(primitives); i++) {
		float distance = dx::XMVectorGetX(dx::XMVector3Length(dx::XMVectorSubtract(
			dx::XMVector4Transform(dx::XMVectorSet(0.0f, 0.8f, 0.0f, 1.0f), manipulator.GetWristTransform()),
			dx::XMLoadFloat3(&scene.objects[primitives[i]].transform.position))
		));
		if (distance < closestDistance) {
			closestDistance = distance;
			closestObject = i;
		}
	}
	if (closestObject != -1 && closestDistance < 0.6f)
		grabbed = primitives[closestObject];
}

bool Simulation::DetectCollision(size_t objectAindex, size_t objectBindex) // Sprawdzenie kolizji dwóch obiektów
{
	OBB a = BuildOBB(objectAindex), b = BuildOBB(objectBindex);

	dx::XMVECTOR T = dx::XMVectorSubtract(dx::XMLoadFloat3(&b.center), dx::XMLoadFloat3(&a.center));

	dx::XMVECTOR aAxes[3] = { dx::XMLoadFloat3(&a.axes[0]),
						      dx::XMLoadFloat3(&a.axes[1]),
						      dx::XMLoadFloat3(&a.axes[2]) };
	dx::XMVECTOR bAxes[3] = { dx::XMLoadFloat3(&b.axes[0]),
						      dx::XMLoadFloat3(&b.axes[1]),
						      dx::XMLoadFloat3(&b.axes[2]) };

	float R[3][3], AbsR[3][3]; // tworzenie macierzy rotacji, czyli macierzy iloczynów skalarnych
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++) {
			dx::XMStoreFloat(&R[i][j], dx::XMVector3Dot(aAxes[i], bAxes[j]));
			AbsR[i][j] = fabsf(R[i][j]) + 1e-6f;
		}

	float ra, rb;

	for (int i = 0; i < 3; i++) { // sprawdzenie osi objektu a
		float t; dx::XMStoreFloat(&t, dx::XMVector3Dot(T, aAxes[i]));
		ra = (&a.halfExtents.x)[i];
		rb = b.halfExtents.x * AbsR[i][0]
		   + b.halfExtents.y * AbsR[i][1]
		   + b.halfExtents.z * AbsR[i][2];
		if (fabsf(t) > ra + rb) return false;
	}

	for (int j = 0; j < 3; j++) { // sprawdzenie osi objektu b
		float t; dx::XMStoreFloat(&t, dx::XMVector3Dot(T, bAxes[j]));
		ra = a.halfExtents.x * AbsR[0][j]
			+ a.halfExtents.y * AbsR[1][j]
			+ a.halfExtents.z * AbsR[2][j];
		rb = (&b.halfExtents.x)[j];
		if (fabsf(t) > ra + rb) return false;
	}

	return true;
}

OBB Simulation::BuildOBB(size_t objectIndex)
{
	OBB obb;
	obb.center = scene.objects[objectIndex].transform.position;
	dx::XMVECTOR q = dx::XMQuaternionNormalize(dx::XMLoadFloat4(&scene.objects[objectIndex].transform.rotation));
	dx::XMStoreFloat3(&obb.axes[0], dx::XMVector3Rotate(dx::XMVectorSet(1, 0, 0, 0), q));
	dx::XMStoreFloat3(&obb.axes[1], dx::XMVector3Rotate(dx::XMVectorSet(0, 1, 0, 0), q));
	dx::XMStoreFloat3(&obb.axes[2], dx::XMVector3Rotate(dx::XMVectorSet(0, 0, 1, 0), q));
	dx::XMStoreFloat3(&obb.halfExtents,
		dx::XMVectorScale(dx::XMLoadFloat3(&scene.objects[objectIndex].transform.scale), 0.5f));
	return obb;
}