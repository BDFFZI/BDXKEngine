#include "GL.h"
#include <exception>
#include "BDXKEngine/Platform/GL/Resources/Texture2D.h"
#include "BDXKEngine/Platform/GL/Resources/TextureCube.h"
#include "BDXKEngine/Platform/GL/Resources/Mesh.h"
#include "BDXKEngine/Platform/GL/Resources/Buffer.h"
#include "BDXKEngine/Platform/GL/Resources/Shader.h"

namespace BDXKEngine
{
    CComPtr<ID3D11Device> GL::GetDevice()
    {
        return device;
    }
    CComPtr<ID3D11DeviceContext> GL::GetDeviceContext()
    {
        return context;
    }
    ObjectPtr<Texture2D> GL::GetRenderTarget()
    {
        return renderTexture;
    }

    // 设置当前渲染管线中用于的着色器
    void GL::SetShader(ObjectPtr<Shader> shader)
    {
        context->IASetInputLayout(shader->inputLayout); //有关顶点着色器的输入布局
        context->VSSetShader(shader->vertexShader, nullptr, 0);
        context->PSSetShader(shader->pixelShader, nullptr, 0);

        //设置管线渲染选项
        context->OMSetBlendState(shader->blendState, nullptr, 0xffffffff);
        context->OMSetDepthStencilState(shader->depthStencilState, 0);
    }
    // 设置当前渲染管线中的顶点索引数据
    void GL::SetMesh(ObjectPtr<Mesh> mesh)
    {
        //绑定顶点数据
        constexpr unsigned int stride = sizeof(Vertex);
        constexpr unsigned int offset = 0;
        context->IASetVertexBuffers(0, 1, &mesh->vertexBuffer->glBuffer.p, &stride, &offset);

        //绑定索引数据
        context->IASetIndexBuffer(mesh->triangleBuffer->glBuffer, DXGI_FORMAT_R16_UINT, 0);
    }
    // 设置当前渲染管线中的常量缓冲区
    void GL::SetBuffer(unsigned int startSlot, ObjectPtr<Buffer> buffer)
    {
        ID3D11Buffer* d3dBuffer = nullptr;
        if (buffer.IsNull() == false)
        {
            if (buffer->target != BufferTarget::Constant)
                throw std::exception("不支持除常量缓冲区以外的类型");
            d3dBuffer = buffer->glBuffer;
        }

        context->VSSetConstantBuffers(startSlot, 1, &d3dBuffer);
        context->PSSetConstantBuffers(startSlot, 1, &d3dBuffer);
    }
    // 设置当前渲染管线中的着色器资源
    void GL::SetTexture(unsigned int startSlot, ObjectPtr<Texture> texture)
    {
        ID3D11ShaderResourceView* resourceView = nullptr;
        ID3D11SamplerState* samplerState = nullptr;
        if (texture.IsNull() == false)
        {
            resourceView = texture->GetResourceView().p;
            samplerState = texture->GetSamplerState().p;
        }

        context->PSSetShaderResources(startSlot, 1, &resourceView);
        context->PSSetSamplers(startSlot, 1, &samplerState);
    }
    // 设置渲染到的目标纹理
    void GL::SetRenderTarget(ObjectPtr<Texture2D> renderTexture)
    {
        GL::renderTexture = renderTexture;
        if (renderTexture.IsNull() == false)
        {
            renderTargetView = renderTexture->renderTextureRTV;
            depthStencilView = renderTexture->depthTextureDSV;
            //调整视口数据至与新纹理大小一致
            D3D11_VIEWPORT viewport = {};
            viewport.Height = static_cast<float>(renderTexture->GetHeight());
            viewport.Width = static_cast<float>(renderTexture->GetWidth());
            viewport.MinDepth = 0;
            viewport.MaxDepth = 1;
            context->RSSetViewports(1, &viewport);
        }
        else //为空时使用默认值
        {
            renderTargetView = defaultRenderTargetView;
            depthStencilView = defaultDepthStencilView;
            //调整视口数据至与新纹理大小一致
            D3D11_VIEWPORT viewport = {};
            viewport.Height = static_cast<float>(defaultRenderTargetDescription.Height);
            viewport.Width = static_cast<float>(defaultRenderTargetDescription.Width);
            viewport.MinDepth = 0;
            viewport.MaxDepth = 1;
            context->RSSetViewports(1, &viewport);
        }

        //设置渲染目标
        context->OMSetRenderTargets(1, &renderTargetView.p, depthStencilView);
    }
    void GL::SetRenderTarget(ObjectPtr<TextureCube> textureCube, int index)
    {
        if (textureCube.IsNull() == false)
        {
            renderTargetView = textureCube->renderTextureRTV[index];
            depthStencilView = textureCube->depthTextureDSV;
            //调整视口数据至与新纹理大小一致
            D3D11_VIEWPORT viewport = {};
            viewport.Height = static_cast<float>(textureCube->GetHeight());
            viewport.Width = static_cast<float>(textureCube->GetWidth());
            viewport.MinDepth = 0;
            viewport.MaxDepth = 1;
            context->RSSetViewports(1, &viewport);
        }
        else //未空时使用默认值
        {
            renderTargetView = defaultRenderTargetView;
            depthStencilView = defaultDepthStencilView;
            //调整视口数据至与新纹理大小一致
            D3D11_VIEWPORT viewport = {};
            viewport.Height = static_cast<float>(defaultRenderTargetDescription.Height);
            viewport.Width = static_cast<float>(defaultRenderTargetDescription.Width);
            viewport.MinDepth = 0;
            viewport.MaxDepth = 1;
            context->RSSetViewports(1, &viewport);
        }

        //设置渲染目标
        context->OMSetRenderTargets(1, &renderTargetView.p, depthStencilView);
    }

    void GL::Clear(bool clearDepth, bool clearColor, Color backgroundColor, float depth)
    {
        //重置绘制视图
        if (clearColor == true)
        {
            const float teal[] = {backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a};
            context->ClearRenderTargetView(renderTargetView, teal);
        }

        //重置深度模板测试视图
        if (clearDepth)
        {
            context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, 0);
        }
    }
    void GL::Render(int indexsCount)
    {
        //开始绘制
        context->DrawIndexed(indexsCount, 0, 0);
    }

    void GL::Initialize(const Window& window)
    {
        CreateDevice();
        CreateSwapChain(window.GetHwnd());
        ResizeDefaultRenderTarget(window.GetScreenRect().GetSize());
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

    CComPtr<ID3D11Device> GL::device = nullptr;
    CComPtr<ID3D11DeviceContext> GL::context = nullptr;
    CComPtr<IDXGISwapChain1> GL::swapChain = nullptr;
    D3D11_TEXTURE2D_DESC GL::defaultRenderTargetDescription = {};
    CComPtr<ID3D11RenderTargetView> GL::defaultRenderTargetView = nullptr;
    CComPtr<ID3D11DepthStencilView> GL::defaultDepthStencilView = nullptr;
    CComPtr<ID3D11RenderTargetView> GL::renderTargetView = nullptr;
    CComPtr<ID3D11DepthStencilView> GL::depthStencilView = nullptr;
    ObjectPtr<Texture2D> GL::renderTexture = nullptr;

    void GL::CreateDevice()
    {
        D3D_FEATURE_LEVEL levels[] = {
            D3D_FEATURE_LEVEL_11_1,
        };

        const HRESULT result = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            D3D11_CREATE_DEVICE_SINGLETHREADED
            | D3D11_CREATE_DEVICE_BGRA_SUPPORT
            | D3D11_CREATE_DEVICE_DEBUG,
            levels,
            ARRAYSIZE(levels),
            D3D11_SDK_VERSION,
            &device,
            nullptr,
            &context
        );
        if (FAILED(result))throw std::exception("Direct3D设备创建失败");
    }
    void GL::CreateSwapChain(HWND hwnd)
    {
        //获取底层DXGI的工厂
        CComPtr<IDXGIDevice> dxglDevice;
        CComPtr<IDXGIAdapter> dxglAdapter;
        CComPtr<IDXGIFactory2> dxglFactory;

        if (FAILED(device->QueryInterface(&dxglDevice)))throw std::exception("获取DXGI设备失败");
        if (FAILED(dxglDevice->GetAdapter(&dxglAdapter)))throw std::exception("获取DXGI适配器失败");
        if (FAILED(dxglAdapter->GetParent(IID_PPV_ARGS(&dxglFactory))))throw std::exception("获取DXGI工厂失败");

        UINT m4xMsaaQuality;
        device->CheckMultisampleQualityLevels(DXGI_FORMAT_B8G8R8A8_UNORM, 4, &m4xMsaaQuality);
        if (m4xMsaaQuality <= 0)throw std::exception("无法满足资源需求级别");


        //交换链的属性配置
        DXGI_SWAP_CHAIN_DESC1 swapChainProperty = {}; //局部变量一定要初始化
        swapChainProperty.BufferCount = 2;
        swapChainProperty.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        swapChainProperty.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainProperty.SampleDesc.Count = 1;
        swapChainProperty.SampleDesc.Quality = 0;
        swapChainProperty.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc = {};
        fullScreenDesc.Windowed = true;

        //创建交换链
        if (FAILED(dxglFactory->CreateSwapChainForHwnd(
                device,
                hwnd,
                &swapChainProperty,
                &fullScreenDesc,
                nullptr,
                &swapChain)
        ))
            throw std::exception("交换链创建失败");
    }

    CComPtr<ID3D11Texture2D> GL::GetDefaultRenderTarget()
    {
        CComPtr<ID3D11Texture2D> renderTargetTexture;
        swapChain->GetBuffer(0, IID_PPV_ARGS(&renderTargetTexture));
        return renderTargetTexture;
    }

    void GL::ResizeDefaultRenderTarget(Vector2 size)
    {
        //需要重置纹理大小，但视图一直占用着纹理而导致没法重置，所以要先释放视图
        if (renderTargetView == defaultRenderTargetView)renderTargetView = nullptr;
        if (depthStencilView == defaultDepthStencilView)depthStencilView = nullptr;
        defaultRenderTargetView = nullptr;
        defaultDepthStencilView = nullptr;

        //重置染目标纹理大小
        swapChain->ResizeBuffers(2, static_cast<UINT>(size.x), static_cast<UINT>(size.y), DXGI_FORMAT_B8G8R8A8_UNORM, 0);
        //获取新的渲染目标纹理及其参数
        CComPtr<ID3D11Texture2D> targetTexture = nullptr;
        swapChain->GetBuffer(0, IID_PPV_ARGS(&targetTexture.p));
        targetTexture->GetDesc(&defaultRenderTargetDescription);
        //以该参数创建用于深度模板测试的纹理
        CComPtr<ID3D11Texture2D> depthStencilTexture = nullptr;
        D3D11_TEXTURE2D_DESC depthStencilTextureDescription = {};
        depthStencilTextureDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilTextureDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthStencilTextureDescription.Width = static_cast<UINT>(defaultRenderTargetDescription.Width);
        depthStencilTextureDescription.Height = static_cast<UINT>(defaultRenderTargetDescription.Height);
        depthStencilTextureDescription.ArraySize = 1;
        depthStencilTextureDescription.SampleDesc = {1, 0};
        HRESULT result = device->CreateTexture2D(&depthStencilTextureDescription, nullptr, &depthStencilTexture.p);
        if (FAILED(result))throw std::exception("创建深度模板测试纹理失败");
        //创建渲染目标纹理视图
        const CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{D3D11_RTV_DIMENSION_TEXTURE2D};
        result = device->CreateRenderTargetView(targetTexture, &renderTargetViewDesc, &defaultRenderTargetView.p);
        if (FAILED(result))throw std::exception("创建渲染目标纹理视图失败");
        //创建深度模板测试的视图
        const CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{D3D11_DSV_DIMENSION_TEXTURE2D};
        result = device->CreateDepthStencilView(depthStencilTexture, &depthStencilViewDesc, &defaultDepthStencilView.p);
        if (FAILED(result))throw std::exception("创建深度模板测试视图失败");
    }
    void GL::Present()
    {
        swapChain->Present(0, 0);
    }
}
