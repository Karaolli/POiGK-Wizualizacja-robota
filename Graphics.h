#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

struct Viewport {
    float x, y, width, height;
};

class Graphics {
public:
    Graphics(HWND hwnd);
	void OnResize();
    void SetViewport(const Viewport& viewport);
    void BeginFrame();
	void ClearColor(const dx::XMFLOAT4& color); // Wyczyszczenie bufora renderowania
	void ClearDepth(); // Wyczyszczenie bufora głębokości
    void EndFrame(UINT vsync); // Pokazanie kadru
    ID3D11Device*        GetDevice()      const { return device.Get(); };
    ID3D11DeviceContext* GetContext()     const { return deviceContext.Get(); };

	UINT GetWidth()  const { return width; }
	UINT GetHeight() const { return height; }
private:
    wrl::ComPtr<ID3D11Device>            device;
    wrl::ComPtr<ID3D11DeviceContext>     deviceContext;
    wrl::ComPtr<IDXGISwapChain>          swapChain;
    wrl::ComPtr<ID3D11DepthStencilState> depthStencilState;
    wrl::ComPtr<ID3D11Texture2D>         depthStencilBuffer;
    wrl::ComPtr<ID3D11DepthStencilView>  depthStencilView;
    wrl::ComPtr<ID3D11RenderTargetView>  renderTargetView;
    wrl::ComPtr<ID3DBlob>                blob;
    wrl::ComPtr<ID3D11VertexShader>      vertexShader;
    wrl::ComPtr<ID3D11PixelShader>       pixelShader;
    wrl::ComPtr<ID3D11InputLayout>       inputLayout;

	UINT width;
	UINT height;
};