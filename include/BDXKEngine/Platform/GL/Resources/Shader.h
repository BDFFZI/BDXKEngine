#pragma once
#include "BDXKEngine/Platform/GL/GL.h"
#include "BDXKEngine/Base/Object/ObjectPtr.h"
#include "GLLayout.h"
#include "BDXKEngine/Platform/GL/Commands/Blend.h"
#include "BDXKEngine/Platform/GL/Commands/ZTest.h"

namespace BDXKEngine
{
    enum class PassType
    {
        ForwardBase,
        ForwardAdd,
        ShadowCaster
    };

    class Shader : public Object, GL
    {
        friend GL;
    public:
        static ObjectPtr<Shader> Create(std::string vertexShaderhlsl, std::string pixelShaderhlsl, PassType passType);

        PassType GetPassType() const;
        Blend GetBlend() const;
        ZTest GetZTest() const;

        void SetPassType(PassType passType);
        void SetBlend(Blend blend);
        void SetZTest(ZTest zTest);
    private:
        std::string vertexShaderhlsl;
        std::string pixelShaderhlsl;
        PassType passType = PassType::ForwardBase;
        Blend blend = Blend::Opaque;
        ZTest zTest = ZTest::Default;

        CComPtr<ID3D11BlendState> blendState;
        CComPtr<ID3D11DepthStencilState> depthStencilState;

        CComPtr<ID3D11InputLayout> inputLayout;
        CComPtr<ID3D11VertexShader> vertexShader;
        CComPtr<ID3D11PixelShader> pixelShader;

        void Transfer(transferer& transferer) override;
        void PreAwake() override;
    };
}
