#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

class Graphics {
public:
    Graphics(HWND hwnd, UINT width, UINT height);
	void OnResize(UINT width, UINT height);
    void BeginFrame();
    void EndFrame(UINT vsync);
    ID3D11Device*        GetDevice()      const { return device.Get(); };
    ID3D11DeviceContext* GetContext()     const { return deviceContext.Get(); };
	IDXGISwapChain*      GetSwapChain()   const { return swapChain.Get(); };
private:
    wrl::ComPtr<ID3D11Device>           device;
    wrl::ComPtr<ID3D11DeviceContext>    deviceContext;
    wrl::ComPtr<IDXGISwapChain>         swapChain;
    wrl::ComPtr<ID3D11DepthStencilState> depthStencilState;
    wrl::ComPtr<ID3D11Texture2D>         depthStencilBuffer;
    wrl::ComPtr<ID3D11DepthStencilView>  depthStencilView;
    wrl::ComPtr<ID3D11RenderTargetView> renderTargetView;
    wrl::ComPtr<ID3DBlob>               blob;
    wrl::ComPtr<ID3D11VertexShader>     vertexShader;
    wrl::ComPtr<ID3D11PixelShader>      pixelShader;
    wrl::ComPtr<ID3D11InputLayout>      inputLayout;
};