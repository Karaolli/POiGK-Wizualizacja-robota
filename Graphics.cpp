#include "Graphics.h"

#include <windows.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <iterator>

Graphics::Graphics(HWND hwnd) {
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 2u;
    scd.BufferDesc.Width = 0;
    scd.BufferDesc.Height = 0;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hwnd;
    scd.SampleDesc.Count = 1u;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG,
        nullptr, 0u, D3D11_SDK_VERSION,
        &scd, &swapChain, &device, nullptr, &deviceContext
    );

    D3D11_DEPTH_STENCIL_DESC dsStateDesc = {};
    dsStateDesc.DepthEnable = TRUE;
    dsStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

    device->CreateDepthStencilState(&dsStateDesc, &depthStencilState);

    deviceContext->OMSetDepthStencilState(depthStencilState.Get(), 0);

    OnResize();

    const D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                             0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    D3DReadFileToBlob(L"assets/shaders/PixelShader.cso", &blob);
    device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader);
    deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0u);

    D3DReadFileToBlob(L"assets/shaders/VertexShader.cso", &blob);
    device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader);
    deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0u);

    device->CreateInputLayout(
        ied, (UINT)std::size(ied),
        blob->GetBufferPointer(),
        blob->GetBufferSize(),
        &inputLayout
    );
    deviceContext->IASetInputLayout(inputLayout.Get());

    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
void Graphics::OnResize() {
    deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	deviceContext->Flush();

    renderTargetView.Reset();
    depthStencilView.Reset();
    depthStencilBuffer.Reset();

    swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

    DXGI_SWAP_CHAIN_DESC desc;
    swapChain->GetDesc(&desc);
	width  = desc.BufferDesc.Width;
	height = desc.BufferDesc.Height;

    ID3D11Texture2D* backBuffer = nullptr;
    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
    device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
    backBuffer->Release();

    D3D11_VIEWPORT vp;
    vp.Width  = width;
    vp.Height = height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    deviceContext->RSSetViewports(1u, &vp);

    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width  = width;
    depthDesc.Height = height;
    depthDesc.MipLevels = 1u;
    depthDesc.ArraySize = 1u;
    depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    device->CreateTexture2D(&depthDesc, nullptr, &depthStencilBuffer);

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;

    device->CreateDepthStencilView(depthStencilBuffer.Get(), &dsvDesc, &depthStencilView);
}
void Graphics::SetViewport(const Viewport& viewport) {
    D3D11_VIEWPORT vp;
    vp.Width  = viewport.width  * width;
    vp.Height = viewport.height * height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = viewport.x * width;
    vp.TopLeftY = viewport.y * height;
    deviceContext->RSSetViewports(1u, &vp);
}
void Graphics::BeginFrame() {
    deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());
}
void Graphics::ClearColor(const dx::XMFLOAT4& color)
{
    const float clearColor[4] = { color.x, color.y, color.z, color.w };
    deviceContext->ClearRenderTargetView(renderTargetView.Get(), clearColor);
}
void Graphics::ClearDepth()
{
    deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}
void Graphics::EndFrame(UINT vsync) {
    SetViewport({ 0.0f, 0.0f, 1.0f, 1.0f });
    swapChain->Present(vsync, 0u);
}