#pragma once

#include <DirectXMath.h>

#include "Graphics.h"

struct TransformData {
    dx::XMMATRIX transform = dx::XMMatrixIdentity();
    dx::XMMATRIX world     = dx::XMMatrixIdentity();
};

struct LightData {
    DirectX::XMFLOAT3 position       = { 0.0f, 3.0f, 0.0f };
    float pad1 = 0.0f;
    DirectX::XMFLOAT3 color          = { 1.0f, 1.0f, 1.0f };
    float pad2 = 0.0f;
    DirectX::XMFLOAT3 cameraPosition = { 0.0f, 0.0f, 5.0f };
    float pad3 = 0.0f;
};


template<typename T>
class ConstantBuffer
{
public:
    T data;
    ConstantBuffer(Graphics& gfx);
	void Upload(Graphics& gfx) { // Przesłanie danych do GPU
		D3D11_MAPPED_SUBRESOURCE mapped;
		gfx.GetContext()->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
		memcpy(mapped.pData, &data, sizeof(T));
		gfx.GetContext()->Unmap(buffer.Get(), 0);
	}
	void BindVS(Graphics& gfx, UINT slot) const {
		gfx.GetContext()->VSSetConstantBuffers(slot, 1u, buffer.GetAddressOf());
	}
    void BindPS(Graphics& gfx, UINT slot) const {
        gfx.GetContext()->PSSetConstantBuffers(slot, 1u, buffer.GetAddressOf());
    }
private:
    wrl::ComPtr<ID3D11Buffer> buffer;
};

template<typename T>
inline ConstantBuffer<T>::ConstantBuffer(Graphics& gfx)
{
    D3D11_BUFFER_DESC cbd = {};
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbd.ByteWidth = sizeof(T);

    gfx.GetDevice()->CreateBuffer(&cbd, nullptr, &buffer);
}