#pragma once

#include "Graphics.h"

struct Vertex
{
    dx::XMFLOAT3 position;
    dx::XMFLOAT3 normal;
};

class Mesh {
public:
    void Load(ID3D11Device* device, // Ładowanie siatki do GPU
        Vertex* vertices, UINT vertexCount,
        unsigned short* indices, UINT indexCount);

    void Bind(Graphics& gfx) const; // Podłączenie buforów przed rysowaniem
	void Draw(Graphics& gfx) const; // Rysowanie na GPU
private:
    wrl::ComPtr<ID3D11Buffer> vertexBuffer;
    wrl::ComPtr<ID3D11Buffer> indexBuffer;
    UINT indexCount;
};