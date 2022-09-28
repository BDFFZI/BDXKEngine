#include "Shader.h"
#include <d3dcompiler.h>

#include <utility>

namespace BDXKEngine
{
    void CompileShader(const wchar_t* path, const char* entrypoint, const char* object, ID3DBlob** blob)
    {
        CComPtr<ID3DBlob> compileError;
        const HRESULT result = D3DCompileFromFile(path,
                                                  nullptr,
                                                  D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                                  entrypoint,
                                                  object,
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

    ObjectPtr<Shader> Shader::Create(std::wstring vertexShaderhlsl, std::wstring pixelShaderhlsl, PassType passType)
    {
        Shader shader = {};
        shader.vertexShaderhlsl = std::move(vertexShaderhlsl);
        shader.pixelShaderhlsl = std::move(pixelShaderhlsl);
        shader.passType = passType;

        return Instantiate<Shader>(&shader);
    }

    PassType Shader::GetPassType() const
    {
        return passType;
    }
    Blend Shader::GetBlend() const
    {
        return blend;
    }
    ZTest Shader::GetZTest() const
    {
        return zTest;
    }

    void Shader::SetPassType(PassType passType)
    {
        this->passType = passType;
    }
    void Shader::SetBlend(Blend blend)
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
        const HRESULT result = device->CreateBlendState(&blendDescription, &blendState.p);
        assert(SUCCEEDED(result));
    }
    void Shader::SetZTest(ZTest zTest)
    {
        depthStencilState = nullptr;
        this->zTest = zTest;
        D3D11_DEPTH_STENCIL_DESC description{};
        description.DepthEnable = true;
        description.DepthWriteMask = zTest.write ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
        description.DepthFunc = static_cast<D3D11_COMPARISON_FUNC>(zTest.operation);
        const HRESULT result = device->CreateDepthStencilState(&description, &depthStencilState.p);
        assert(SUCCEEDED(result));
    }

    void Shader::Transfer(Transferrer& transferrer)
    {
        Object::Transfer(transferrer);

        transferrer.TransferString(nameof(vertexShaderhlsl), vertexShaderhlsl);
        transferrer.TransferString(nameof(pixelShaderhlsl), pixelShaderhlsl);
        transferrer.TransferInt(nameof(passType), &passType);
        transferrer.TransferBytes(nameof(blend), &blend, sizeof(Blend));
        transferrer.TransferBytes(nameof(zTest), &zTest, sizeof(ZTest));
    }
    void Shader::Awake()
    {
        Object::Awake();

        HRESULT result = {};
        //编译顶点着色器
        CComPtr<ID3DBlob> vertexBlob;
        CompileShader(vertexShaderhlsl.c_str(), "main", "vs_5_0", &vertexBlob.p);
        //创建顶点着色器
        result = device->CreateVertexShader(
            vertexBlob->GetBufferPointer(),
            vertexBlob->GetBufferSize(),
            nullptr,
            &vertexShader.p
        );
        assert(SUCCEEDED(result));

        //编译像素着色器
        CComPtr<ID3DBlob> pixelBlob;
        CompileShader(pixelShaderhlsl.c_str(), "main", "ps_5_0", &pixelBlob.p);
        //创建像素着色器
        result = device->CreatePixelShader(
            pixelBlob->GetBufferPointer(),
            pixelBlob->GetBufferSize(),
            nullptr,
            &pixelShader.p
        );
        assert(SUCCEEDED(result));

        //创建语义信息
        result = device->CreateInputLayout(
            VertexDescription,
            ARRAYSIZE(VertexDescription),
            vertexBlob->GetBufferPointer(),
            vertexBlob->GetBufferSize(),
            &inputLayout.p
        );
        assert(SUCCEEDED(result));

        SetPassType(passType);
        //初始化管线选项
        SetBlend(blend);
        SetZTest(zTest);
    }
}
