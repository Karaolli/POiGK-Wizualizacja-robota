#include "Mesh.h"

Mesh::Mesh(wrl::ComPtr<ID3D11Device> device, Vertex* vertices, UINT vertexCount, unsigned short* indices, UINT indexCount) {
    this->indexCount = indexCount;

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.ByteWidth = sizeof(Vertex) * vertexCount;
    bd.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices;

    device->CreateBuffer(&bd, &sd, &vertexBuffer);


    D3D11_BUFFER_DESC ibd = {};
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.ByteWidth = sizeof(unsigned short) * indexCount;

    D3D11_SUBRESOURCE_DATA isd = {};
    isd.pSysMem = indices;

    device->CreateBuffer(&ibd, &isd, &indexBuffer);
}
void Mesh::Bind(wrl::ComPtr<ID3D11DeviceContext> deviceContext) {
    const UINT stride = sizeof(Vertex);
    const UINT offset = 0u;
    deviceContext->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);
    deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
}
UINT Mesh::GetIndexCount() const {
    return indexCount;
}