#pragma once
#include "../Core/GL.h"
#include "BDXKEngine/Base/Object/Core/Object.h"
#include "BDXKEngine/Base/Reflection/Reflection.h"

namespace BDXKEngine
{
    class Shader : public Object
    {
    public:
        static ObjectPtr<Shader> Create(const std::string& vertexShaderhlsl, const std::string& pixelShaderhlsl);
        static ObjectPtr<Shader> Create(const CComPtr<ID3DBlob>& vertexBlob, const CComPtr<ID3DBlob>& pixelBlob);

        void UploadRP() const; // 设置当前渲染管线中用于的着色器
    private:
        std::vector<char> vertexPass;
        std::vector<char> pixelPass;

        CComPtr<ID3D11VertexShader> vertexShader;
        CComPtr<ID3D11PixelShader> pixelShader;
        CComPtr<ID3D11InputLayout> inputLayout;

        void Transfer(Transferer& transferer) override;
        void Awake() override;
    };

    CustomReflection(Shader)
}
