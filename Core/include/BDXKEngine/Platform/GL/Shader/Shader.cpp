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

    void Shader::UploadRP() const
    {
        const auto context = GL::GetDeviceContext();
        context->IASetInputLayout(inputLayout); //有关顶点着色器的输入布局
        context->VSSetShader(vertexShader, nullptr, 0);
        context->PSSetShader(pixelShader, nullptr, 0);
    }

    void Shader::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

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
    }
}
