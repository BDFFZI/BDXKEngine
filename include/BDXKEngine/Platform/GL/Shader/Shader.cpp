#include "Shader.h"
#include <d3dcompiler.h>
#include "BDXKEngine/Platform/GL/Core/GLLayout.h"

namespace BDXKEngine
{
    void CompileShader(const std::string& path, const char* entrypoint, const char* target, ID3DBlob** blob)
    {
        CComPtr<ID3DBlob> compileError;
        const HRESULT result = D3DCompile(path.c_str(),
                                          path.size(),
                                          nullptr,
                                          nullptr,
                                          nullptr,
                                          entrypoint,
                                          target,
                                          D3DCOMPILE_DEBUG,
                                          0,
                                          blob,
                                          &compileError.p
        );

        if (FAILED(result))
        {
            // ReSharper disable once CppDeclaratorNeverUsed
            char* errorInfo = static_cast<char*>(compileError->GetBufferPointer());
            throw std::exception("编译着色器失败");
        }
    }

    ObjectPtr<Shader> Shader::Create(const std::string& vertexShaderhlsl, const std::string& pixelShaderhlsl)
    {
        ObjectPtr shader = new Shader{};

        //编译顶点着色器
        CComPtr<ID3DBlob> vertexBlob;
        CompileShader(vertexShaderhlsl, "main", "vs_5_0", &vertexBlob.p);
        shader->vertexPass.resize(vertexBlob->GetBufferSize());
        memcpy_s(shader->vertexPass.data(), shader->vertexPass.size(),
                 vertexBlob->GetBufferPointer(), shader->vertexPass.size()
        );

        //编译像素着色器
        CComPtr<ID3DBlob> pixelBlob;
        CompileShader(pixelShaderhlsl, "main", "ps_5_0", &pixelBlob.p);
        shader->pixelPass.resize(pixelBlob->GetBufferSize());
        memcpy_s(shader->pixelPass.data(), shader->pixelPass.size(),
                 pixelBlob->GetBufferPointer(), shader->pixelPass.size()
        );

        Instantiate(shader);

        return shader;
    }
    ObjectPtr<Shader> Shader::Create(const CComPtr<ID3DBlob>& vertexBlob, const CComPtr<ID3DBlob>& pixelBlob)
    {
        ObjectPtr shader = new Shader{};

        shader->vertexPass.resize(vertexBlob->GetBufferSize());
        memcpy_s(shader->vertexPass.data(), shader->vertexPass.size(),
                 vertexBlob->GetBufferPointer(), shader->vertexPass.size()
        );
        shader->pixelPass.resize(pixelBlob->GetBufferSize());
        memcpy_s(shader->pixelPass.data(), shader->pixelPass.size(),
                 pixelBlob->GetBufferPointer(), shader->pixelPass.size()
        );
        
        Instantiate(shader);

        return shader;
    }

    const Cull& Shader::GetCull() const
    {
        return cull;
    }
    const ZTest& Shader::GetZTest() const
    {
        return zTest;
    }
    const Blend& Shader::GetBlend() const
    {
        return blend;
    }

    void Shader::SetCull(const Cull& cull)
    {
        rasterizerState = nullptr;
        this->cull = cull;
        D3D11_RASTERIZER_DESC description;
        description.FillMode = D3D11_FILL_SOLID;
        description.CullMode = static_cast<D3D11_CULL_MODE>(cull.state);
        description.FrontCounterClockwise = false;
        description.DepthBias = 0;
        description.DepthBiasClamp = 0;
        description.SlopeScaledDepthBias = 0;
        description.DepthClipEnable = true;
        description.ScissorEnable = false;
        description.MultisampleEnable = false;
        description.AntialiasedLineEnable = false;
        GL::GetDevice()->CreateRasterizerState(&description, &rasterizerState.p);
    }
    void Shader::SetZTest(const ZTest& zTest)
    {
        depthStencilState = nullptr;
        this->zTest = zTest;
        D3D11_DEPTH_STENCIL_DESC description{};
        description.DepthEnable = true;
        description.DepthWriteMask = zTest.write ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
        description.DepthFunc = static_cast<D3D11_COMPARISON_FUNC>(zTest.operation);
        const HRESULT result = GL::GetDevice()->CreateDepthStencilState(&description, &depthStencilState.p);
        if (FAILED(result))throw std::exception("创建深度测试参数失败");
    }
    void Shader::SetBlend(const Blend& blend)
    {
        blendState = nullptr;
        this->blend = blend;
        D3D11_RENDER_TARGET_BLEND_DESC renderTargetBlendDescription;
        renderTargetBlendDescription.BlendEnable = blend.state;
        renderTargetBlendDescription.SrcBlend = static_cast<D3D11_BLEND>(blend.sourceFactor);
        renderTargetBlendDescription.DestBlend = static_cast<D3D11_BLEND>(blend.destinationFactor);
        renderTargetBlendDescription.BlendOp = static_cast<D3D11_BLEND_OP>(blend.operation);
        renderTargetBlendDescription.SrcBlendAlpha = D3D11_BLEND_ONE;
        renderTargetBlendDescription.DestBlendAlpha = D3D11_BLEND_ONE;
        renderTargetBlendDescription.BlendOpAlpha = D3D11_BLEND_OP_MAX;
        renderTargetBlendDescription.RenderTargetWriteMask = 15;
        D3D11_BLEND_DESC blendDescription{};
        blendDescription.RenderTarget[0] = renderTargetBlendDescription;
        const HRESULT result = GL::GetDevice()->CreateBlendState(&blendDescription, &blendState.p);
        if (FAILED(result))throw std::exception("创建渲染混合参数失败");
    }
    void Shader::SetPass() const
    {
        const auto context = GL::GetDeviceContext();
        context->IASetInputLayout(inputLayout); //有关顶点着色器的输入布局
        context->VSSetShader(vertexShader, nullptr, 0);
        context->PSSetShader(pixelShader, nullptr, 0);

        //设置管线渲染选项
        context->RSSetState(rasterizerState);
        context->OMSetDepthStencilState(depthStencilState, 0);
        context->OMSetBlendState(blendState, nullptr, 0xffffffff);
    }

    void Shader::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

        TransferNestedInfo(cull);
        TransferNestedInfo(zTest);
        TransferNestedInfo(blend);
        TransferFieldInfo(vertexPass);
        TransferFieldInfo(pixelPass);
    }
    void Shader::Awake()
    {
        Object::Awake();


        //创建顶点着色器
        HRESULT result = GL::GetDevice()->CreateVertexShader(
            vertexPass.data(),
            vertexPass.size(),
            nullptr,
            &vertexShader.p
        );
        if (FAILED(result))throw std::exception("创建顶点着色器失败");
        //创建像素着色器
        result = GL::GetDevice()->CreatePixelShader(
            pixelPass.data(),
            pixelPass.size(),
            nullptr,
            &pixelShader.p
        );
        if (FAILED(result))throw std::exception("创建像素着色器失败");
        //创建语义信息
        result = GL::GetDevice()->CreateInputLayout(
            VertexDescription,
            ARRAYSIZE(VertexDescription),
            vertexPass.data(),
            vertexPass.size(),
            &inputLayout.p
        );
        if (FAILED(result))throw std::exception("创建着色器语义失败");

        //初始化管线选项
        SetCull(cull);
        SetZTest(zTest);
        SetBlend(blend);
    }
}
