#include "Mesh.h"

void Mesh::Load(ID3D11Device* device, Vertex* vertices, UINT vertexCount, unsigned short* indices, UINT indexCount) {
    this->indexCount = indexCount;

	D3D11_BUFFER_DESC bd = {};     // stworzenie bufora wierzchołków
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.ByteWidth = sizeof(Vertex) * vertexCount;
    bd.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = vertices;

    device->CreateBuffer(&bd, &sd, &vertexBuffer);


    D3D11_BUFFER_DESC ibd = {};     // stworzenie bufora indeksów
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.ByteWidth = sizeof(unsigned short) * indexCount;

    D3D11_SUBRESOURCE_DATA isd = {};
    isd.pSysMem = indices;

    device->CreateBuffer(&ibd, &isd, &indexBuffer);
}
void Mesh::Bind(Graphics& gfx) const {
    const UINT stride = sizeof(Vertex);
    const UINT offset = 0u;
    gfx.GetContext()->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);
    gfx.GetContext()->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
}
void Mesh::Draw(Graphics& gfx) const {
    gfx.GetContext()->DrawIndexed(indexCount, 0u, 0);
}