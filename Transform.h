#pragma once

#include <DirectXMath.h>

struct Transform {
    DirectX::XMFLOAT3 position = { 0, 0, 0 };
    DirectX::XMFLOAT4 rotation = { 0, 0, 0, 1 };
    DirectX::XMFLOAT3 scale    = { 1, 1, 1 };

    DirectX::XMMATRIX GetMatrix() const {
        DirectX::XMVECTOR rotationQuaternion =
            DirectX::XMQuaternionNormalize(DirectX::XMLoadFloat4(&rotation));

        DirectX::XMMATRIX matrix =
            DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
            DirectX::XMMatrixRotationQuaternion(rotationQuaternion) *
            DirectX::XMMatrixTranslation(position.x, position.y, position.z);
        return matrix;
    }
	void SetPositionRotationMatrix(const DirectX::XMMATRIX& matrix) {
		DirectX::XMVECTOR scale, rotation, translation;
		DirectX::XMMatrixDecompose(&scale, &rotation, &translation, matrix);
		DirectX::XMStoreFloat3(&this->position, translation);
		DirectX::XMStoreFloat4(&this->rotation, rotation);
	}
};