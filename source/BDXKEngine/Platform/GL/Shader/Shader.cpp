#include "Shader.h"
#include <d3dcompiler.h>
#include <filesystem>

#include "BDXKEngine/Base/Data/String/String.h"
#include "BDXKEngine/Platform/GL/Core/GLLayout.h"

namespace BDXKEngine
{
    void CompileShader(const std::string& hlsl, const char* entrypoint, const char* target, ID3DBlob** blob)
    {
        CComPtr<ID3DBlob> compileError;
        const HRESULT result = D3DCompile(hlsl.c_str(),
                                          hlsl.size(),
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
    void CompileShaderFromFile(const std::string& path, const char* entrypoint, const char* target, ID3DBlob** blob)
    {
        if (std::filesystem::exists(path) == false)
            throw std::exception("文件不存在");

        CComPtr<ID3DBlob> compileError;
        const HRESULT result = D3DCompileFromFile(String::ToWString(path).c_str(),
                                                  nullptr,
                                                  D3D_COMPILE_STANDARD_FILE_INCLUDE,
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
            const char* errorInfo = static_cast<char*>(compileError->GetBufferPointer());
            throw std::exception((std::string("编译着色器失败:") + errorInfo).c_str());
        }
    }

    ObjectPtr<Shader> Shader::Create(const std::string& hlsl)
    {
        ObjectPtr shader = new Shader{};

        //编译顶点着色器
        CComPtr<ID3DBlob> vertexBlob;
        CompileShader(hlsl, "VertexPass", "vs_5_0", &vertexBlob.p);
        shader->vertexPass.resize(vertexBlob->GetBufferSize());
        memcpy_s(shader->vertexPass.data(), shader->vertexPass.size(),
                 vertexBlob->GetBufferPointer(), shader->vertexPass.size()
        );

        //编译像素着色器
        CComPtr<ID3DBlob> pixelBlob;
        CompileShader(hlsl, "PixelPass", "ps_5_0", &pixelBlob.p);
        shader->pixelPass.resize(pixelBlob->GetBufferSize());
        memcpy_s(shader->pixelPass.data(), shader->pixelPass.size(),
                 pixelBlob->GetBufferPointer(), shader->pixelPass.size()
        );

        Instantiate(shader);

        return shader;
    }
    ObjectPtr<Shader> Shader::CreateFromFile(const std::string& path)
    {
        ObjectPtr shader = new Shader{};

        //编译顶点着色器
        CComPtr<ID3DBlob> vertexBlob;
        CompileShaderFromFile(path, "VertexPass", "vs_5_0", &vertexBlob.p);
        shader->vertexPass.resize(vertexBlob->GetBufferSize());
        memcpy_s(shader->vertexPass.data(), shader->vertexPass.size(),
                 vertexBlob->GetBufferPointer(), shader->vertexPass.size()
        );

        //编译像素着色器
        CComPtr<ID3DBlob> pixelBlob;
        CompileShaderFromFile(path, "PixelPass", "ps_5_0", &pixelBlob.p);
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
