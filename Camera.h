#pragma once

#include "Graphics.h"
#include "Input.h"

#include <algorithm>

class Camera {
public:
	dx::XMMATRIX GetMatrix(Graphics& gfx) const {
		float aspectRatio =
			((float)gfx.GetWidth()  * GetViewport().width) /
			((float)gfx.GetHeight() * GetViewport().height);
		return GetView() * dx::XMMatrixPerspectiveFovLH(fov, aspectRatio, nearClip, farClip);
	};
	dx::XMFLOAT3 GetPosition() const { return position; }
	Viewport     GetViewport() const { return viewport; }
	void SetFOV(float fov) { this->fov = fov * dx::XM_PI / 180.0f; }
protected:
	virtual dx::XMMATRIX GetView() const = 0;
	dx::XMFLOAT3 position = { 0.0f, 0.0f, -1.0f };
	Viewport viewport = { 0.0f, 0.0f, 1.0f, 1.0f };
private:
	float fov = 60.0f * dx::XM_PI / 180.0f;
	float nearClip = 0.1f;
	float farClip = 100.0f;
};

class CameraOrbit : public Camera {
public:
	void Update(const Input& input) { // Aktualizacja pozycji kamery myszką
		if (input.mouseButtons[1]) { // Obracanie
			yaw -= input.mouseDX * orbitSensitivity;
			if (yaw >  dx::XM_PI) yaw -= 2.0f * dx::XM_PI;
			if (yaw < -dx::XM_PI) yaw += 2.0f * dx::XM_PI;
			pitch += input.mouseDY * orbitSensitivity;
			pitch = std::clamp(pitch, -1.57f, 1.57f);
		}
		if (input.mouseScrollDelta != 0.0f) { // Zoomowanie
			radius -= input.mouseScrollDelta * scrollSensitivity;
			radius = std::clamp(radius, minRadius, maxRadius);
		}

		float x = radius * cosf(pitch) * sinf(yaw);
		float y = radius * sinf(pitch);
		float z = -radius * cosf(pitch) * cosf(yaw);
		position = { x, y, z };
	};
private:
	dx::XMMATRIX GetView() const override {
		return dx::XMMatrixLookAtLH(XMLoadFloat3(&position), dx::XMLoadFloat3(&target), { 0.0f, 1.0f, 0.0f });
	}
	dx::XMFLOAT3 target = { 0.0f, 0.0f,  0.0f };
	float yaw = 0.0f;
	float pitch = 0.5f;
	float radius = 5.0f;

	float orbitSensitivity = 0.003f;
	float scrollSensitivity = 0.5f;
	float minRadius = 1.0f;
	float maxRadius = 50.0f;
};

class CameraAttached : public Camera {
public:
	CameraAttached() {
		viewport = { 0.7f, 0.7f, 0.3f, 0.3f };
	}
	void Update(dx::XMMATRIX matrix) {
		matrix1 = matrix;
		dx::XMMatrixDecompose(&scale, &rotation, &translation, matrix);
		dx::XMStoreFloat3(&this->position, translation);
	}
private:
	dx::XMMATRIX GetView() const override {
		return dx::XMMatrixInverse(nullptr, matrix1) * dx::XMMatrixRotationX(dx::XM_PIDIV2);
	}
	dx::XMMATRIX matrix1;
	dx::XMVECTOR scale;
	dx::XMVECTOR rotation;
	dx::XMVECTOR translation;
};