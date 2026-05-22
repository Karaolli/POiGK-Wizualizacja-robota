#pragma once

#include <DirectXMath.h>
#include <algorithm>

namespace dx = DirectX;

class Camera {
public:
	dx::XMMATRIX GetMatrix(float aspectRatio) const {
		dx::XMFLOAT3 pos = GetPosition();
		return dx::XMMatrixLookAtLH(XMLoadFloat3(&pos), dx::XMLoadFloat3(&target), {0, 1, 0}) *
			dx::XMMatrixPerspectiveFovLH(45, aspectRatio, 0.1, 100);
	};
	dx::XMFLOAT3 GetPosition() const {
		float x = radius * cosf(pitch) * sinf(yaw);
		float y = radius * sinf(pitch);
		float z = -radius * cosf(pitch) * cosf(yaw);
		return { x, y, z };
	};
	void OnMouseDrag(float dx, float dy) {
		yaw   -= dx * orbitSensitivity;
		pitch += dy * orbitSensitivity;
		pitch = std::clamp(pitch, -1.57f, 1.57f);
	};
	void OnMouseScroll(float delta) {
		radius -= delta * scrollSensitivity;
		radius = std::clamp(radius, minRadius, maxRadius);
	};
private:
	dx::XMFLOAT3 target = { 0.0f, 0.0f, 0.0f };
	float radius = 5.0f;
	float yaw    = 0.0f;
	float pitch  = 0.5f;

	float orbitSensitivity  = 0.003f;
	float scrollSensitivity = 0.5f;
	float minRadius = 1.0f;
	float maxRadius = 50.0f;
};