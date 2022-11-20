#pragma once
#include "../GL.h"
#include "Command/Cull.h"
#include "Command/ZTest.h"
#include "Command/Blend.h"
#include "BDXKEngine/Base/Object/Core/Object.h"
#include "BDXKEngine/Base/Reflection/Reflection.h"

namespace BDXKEngine
{
    class Shader : public Object
    {
    public:
        static ObjectPtr<Shader> Create(const std::string& vertexShaderhlsl, const std::string& pixelShaderhlsl);
        static ObjectPtr<Shader> Create(const CComPtr<ID3DBlob>& vertexBlob, const CComPtr<ID3DBlob>& pixelBlob);

        const ZTest& GetZTest() const;
        const Blend& GetBlend() const;
        const Cull& GetCull() const;

        void SetCull(const Cull& cull);
        void SetZTest(const ZTest& zTest);
        void SetBlend(const Blend& blend);
        void SetPass() const; // 设置当前渲染管线中用于的着色器
    private:
        Cull cull = Cull::Default;
        ZTest zTest = ZTest::Default;
        Blend blend = Blend::Opaque;
        std::vector<char> vertexPass;
        std::vector<char> pixelPass;

        CComPtr<ID3D11VertexShader> vertexShader;
        CComPtr<ID3D11PixelShader> pixelShader;
        CComPtr<ID3D11InputLayout> inputLayout;

        CComPtr<ID3D11DepthStencilState> depthStencilState;
        CComPtr<ID3D11BlendState> blendState;
        CComPtr<ID3D11RasterizerState> rasterizerState;

        void Transfer(Transferer& transferer) override;
        void Awake() override;
    };

    CustomReflection(Shader)
}
