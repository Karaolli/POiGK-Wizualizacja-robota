#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <iterator>
#include <chrono>

#include "Mesh.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3DCompiler.lib")

wrl::ComPtr<ID3D11Device>           device;
wrl::ComPtr<ID3D11DeviceContext>    deviceContext;
wrl::ComPtr<IDXGISwapChain>         swapChain;
wrl::ComPtr<ID3D11RenderTargetView> renderTargetView;
wrl::ComPtr<ID3DBlob>               blob;
wrl::ComPtr<ID3D11VertexShader>     vertexShader;
wrl::ComPtr<ID3D11PixelShader>      pixelShader;
wrl::ComPtr<ID3D11InputLayout>      inputLayout;
wrl::ComPtr<ID3D11Buffer>           constantBuffer;

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

UINT width = 1280u, height = 720u;
std::chrono::duration<double, std::milli> delta;
float yaw = 0.78f, pitch = 0.78f;

Vertex cubeVertices[] = {
    {0.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, 1.0f, 1.0f},
    {1.0f, 0.0f, 0.0f},
    {1.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, 0.0f},
    {1.0f, 1.0f, 1.0f}
};

unsigned short cubeIndices[] =
{
    0, 2, 4,
    2, 6, 4,
    2, 3, 6,
    3, 7, 6,
    3, 1, 7,
    1, 5, 7,
    5, 6, 7,
    5, 4, 6,
    5, 1, 4,
    4, 1, 0,
    1, 3, 0,
    3, 2, 0
};


class ConstantBuffer {
public:
    dx::XMMATRIX transform = dx::XMMatrixIdentity();
} cb;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) { // funkcja przyjmująca instrukcje z zewnątrz, np. kliknięcia myszy, zamknięcie okna
    switch (msg)
    {
    case WM_SIZE: {
        if (swapChain && wParam != SIZE_MINIMIZED) {
            width = LOWORD(lParam);
            height = HIWORD(lParam);

            //deviceContext->OMSetRenderTargets(0, nullptr, nullptr);
            renderTargetView.Reset();

            swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

            ID3D11Texture2D* backBuffer = nullptr;
            swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
            device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
            backBuffer->Release();

            D3D11_VIEWPORT vp;
            vp.Width = width;
            vp.Height = height;
            vp.MinDepth = 0.0f;
            vp.MaxDepth = 1.0f;
            vp.TopLeftX = 0.0f;
            vp.TopLeftY = 0.0f;
            deviceContext->RSSetViewports(1u, &vp);
        }
        return 0;
    }
    case WM_DESTROY: // zamknięcie okna
        PostQuitMessage(0); // zatrzymanie głównej pętli
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam); // pomyślne zachowanie okna
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    WNDCLASSEXW wc = {}; // Klasa okna, zawierająca informacje o tym jakie ono ma być
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = nullptr;
    wc.lpszClassName = L"MainWindow";
    RegisterClassExW(&wc);

    RECT wr = {};
    wr.left = 0;
    wr.right = width + wr.left;
    wr.top = 0;
    wr.bottom = height + wr.top;
    AdjustWindowRectEx(&wr, WS_OVERLAPPEDWINDOW, FALSE, 0);

    HWND hwnd = CreateWindowExW(
        0, L"MainWindow", L"Wizualizacja robota 3000",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
        nullptr, nullptr, hInstance, nullptr
    );
    ShowWindow(hwnd, SW_SHOW);
    
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

    ID3D11Texture2D* backBuffer = nullptr;
    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
    device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
    backBuffer->Release();

    deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);

    const D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    D3DReadFileToBlob(L"PixelShader.cso", &blob);
    device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader);
    deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0u);

    D3DReadFileToBlob(L"VertexShader.cso", &blob);
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

    D3D11_VIEWPORT vp;
    vp.Width = width;
    vp.Height = height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    deviceContext->RSSetViewports(1u, &vp);

    D3D11_BUFFER_DESC cbd = {};
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbd.ByteWidth = sizeof(cb);

    D3D11_SUBRESOURCE_DATA csd = {};
    csd.pSysMem = &cb;

    device->CreateBuffer(&cbd, &csd, &constantBuffer);

    deviceContext->VSSetConstantBuffers(0u, 1u, constantBuffer.GetAddressOf());

    Mesh cubeMesh(device, cubeVertices, (UINT)std::size(cubeVertices), cubeIndices, (UINT)std::size(cubeIndices));

    std::chrono::steady_clock::time_point lastTime = std::chrono::high_resolution_clock::now();

    MSG msg = {};
    
    while (msg.message != WM_QUIT) { // główna pętla, wyłączana przez wywołanie funkcji PostQuitMessage(0)
        if (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)) { // jeżeli jest instrukcja...
            TranslateMessage(&msg);
            DispatchMessage(&msg);                       // ...wyślij ją do funkcji WindowProc()
        }

        std::chrono::steady_clock::time_point now = std::chrono::high_resolution_clock::now();
        delta = now - lastTime;
        lastTime = now;

        if      (GetAsyncKeyState(VK_LEFT) & 0x8000) yaw = yaw - 0.001 * delta.count();
        else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) yaw = yaw + 0.001 * delta.count();

        if      (GetAsyncKeyState(VK_UP) & 0x8000) pitch = pitch - 0.001 * delta.count();
        else if (GetAsyncKeyState(VK_DOWN) & 0x8000) pitch = pitch + 0.001 * delta.count();

        cb.transform = dx::XMMatrixTranspose(
            dx::XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f) *
            dx::XMMatrixLookAtLH({ 0, 0, -5 }, { 0, 0, 1 }, { 0, 1, 0 }) *
            dx::XMMatrixPerspectiveFovLH(45, (float)width / (float)height, 1, 100)
        );

        D3D11_MAPPED_SUBRESOURCE mapped;
        deviceContext->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
        memcpy(mapped.pData, &cb, sizeof(cb));
        deviceContext->Unmap(constantBuffer.Get(), 0);

        deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);

        float color[4] = { 0.05f, 0.05f, 0.2f, 1.0f };
        deviceContext->ClearRenderTargetView(renderTargetView.Get(), color);

        cubeMesh.Bind(deviceContext);
        deviceContext->DrawIndexed(cubeMesh.GetIndexCount(), 0u, 0);


        swapChain->Present(1u, 0u);
    }
}