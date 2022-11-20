#pragma once
#include <d3d11_1.h>
#include <atlbase.h>
#include "BDXKEngine/Base/Data/Graphics/Color.h"
#include "BDXKEngine/Platform/Window/Window.h"

namespace BDXKEngine
{
    class GL
    {
    public:
        static void Initialize(Window* window);

        static CComPtr<ID3D11Device> GetDevice();
        static CComPtr<ID3D11DeviceContext> GetDeviceContext();

        static void SetDefaultRenderTarget();
        static void SetNullTexture2D(unsigned int startSlot);

        //确保渲染前你已经绑定渲染目标
        static void Clear(bool clearDepth, bool clearColor, Color backgroundColor = Color::clear, float depth = 1.0f);
        static void Render(int indexsCount);
    protected:
        static Window* window;
        static CComPtr<ID3D11Device> device;
        static CComPtr<ID3D11DeviceContext> context;
        static CComPtr<IDXGISwapChain1> swapChain;
        //默认资产
        static D3D11_TEXTURE2D_DESC defaultRenderTargetDescription;
        static CComPtr<ID3D11RenderTargetView> defaultRenderTargetView;
        static CComPtr<ID3D11DepthStencilView> defaultDepthStencilView;

        static void CreateDevice();
        static void CreateSwapChain();
        // 每当窗口大小被调整时
        // 若使用了GL2D功能，则可能出现因为渲染纹理被GL2D占用而导致无法重置纹理大小，所以要配合GL2D那边的资源释放
        static void ResizeDefaultRenderTarget();
        /// 将默认渲染目标的画面呈现到屏幕上,每次调用后渲染目标都会自动置空,需要重新绑定
        static void Present();
    };
}
