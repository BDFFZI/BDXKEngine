#pragma once
#include <d3d11_1.h>
#include <atlbase.h>
#include "BDXKEngine/Base/Data/Graphics/Color.h"
#include "BDXKEngine/Platform/Window/Window.h"
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3DCompiler.lib")

namespace BDXKEngine
{
    class GL
    {
    public:
        static void Initialize(Window* window);

        static CComPtr<ID3D11Device> GetDevice();
        static CComPtr<ID3D11DeviceContext> GetDeviceContext();
        static CComPtr<IDXGISwapChain1> GetSwapChain();


        //确保渲染前你已经绑定渲染目标
        static void Clear(bool clearDepth, bool clearColor, Color backgroundColor = Color::clear, float depth = 1.0f);
        static void Render(int indexsCount);
        /// 将默认渲染目标的画面呈现到屏幕上,每次调用后渲染目标都会自动置空,需要重新绑定
        static void Present();
    protected:
        static Window* window;
        static CComPtr<ID3D11Device> device;
        static CComPtr<ID3D11DeviceContext> context;
        static CComPtr<IDXGISwapChain1> swapChain;

        static void CreateDevice();
        static void CreateSwapChain();
    };
}
