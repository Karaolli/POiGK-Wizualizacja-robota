#pragma once

#include <d3d11.h>
#include <wrl/client.h>

namespace wrl = Microsoft::WRL;

struct Vertex
{
    float x, y, z;
};

class Mesh {
public:
    Mesh(wrl::ComPtr<ID3D11Device> device,
        Vertex* vertices, UINT vertexCount,
        unsigned short* indices, UINT indexCount);

    void Bind(wrl::ComPtr<ID3D11DeviceContext> deviceContext);
    UINT GetIndexCount() const;
private:
    wrl::ComPtr<ID3D11Buffer> vertexBuffer;
    wrl::ComPtr<ID3D11Buffer> indexBuffer;
    UINT indexCount;
};