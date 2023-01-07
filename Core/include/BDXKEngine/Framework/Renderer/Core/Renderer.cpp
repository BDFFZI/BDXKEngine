#include "Renderer.h"
#include <algorithm>

namespace BDXKEngine
{
    std::vector<Renderer*> rendererQueue = {};
    std::vector<Renderer*> Renderer::renderers = {};
    ObjectPtr<Material> Renderer::fallbackMaterial = nullptr;

    ObjectPtr<Material> CreateFallbackMaterial()
    {
        ObjectPtr<Shader> fallbackShader = Shader::Create(R"(
            cbuffer MaterialInfo : register(b0)
            {
            float4 float0_3;//metallic,smoothness
            float4 float4_7;
            float4 vector0;
            float4 vector1;
            float4 vector2;
            float4 vector3;
            float4 vector4;
            float4 vector5;
            float4x4 matrix0;//objectToWorld
            float4x4 matrix1;
            float4x4 matrix2;
            float4x4 matrix3;
            };
            cbuffer CameraInfo : register(b1)
            {
            float4x4 WorldToView;
            float4x4 ViewToClip;
            float4 CameraPosition;
            float4 Environment;
            float4 Time;
            };
            float4 VertexPass(float3 positionOS : POSITION):SV_POSITION
            {
                float4 positionCS = float4(positionOS, 1);
                positionCS = mul(matrix0, positionCS);
                positionCS = mul(WorldToView, positionCS);
                positionCS = mul(ViewToClip, positionCS);
                return positionCS;
            }
            float4 PixelPass() : SV_TARGET
            {
                return float4(1,0,1,1);
            }
        )");
        return Material::Create({{PassType::ForwardBase, fallbackShader}});
    }
    const std::vector<Renderer*>& Renderer::GetRendererQueue()
    {
        rendererQueue.clear();

        for (const auto& renderer : renderers)
            if (renderer->GetMesh().IsNotNull())
                rendererQueue.emplace_back(renderer);

        std::ranges::sort(
            rendererQueue,
            [](const Renderer* a, const Renderer* b)
            {
                return a->GetMaterial(true)->GetRenderQueue() < b->GetMaterial(true)->GetRenderQueue();
            }
        );
        return rendererQueue;
    }

    const ObjectPtr<Material>& Renderer::GetMaterial(bool fallback) const
    {
        if (material.IsNotNull())
            material->SetMatrix(0, GetGameObject()->GetLocalToWorldMatrix());
        else if (fallback)
        {
            if (fallbackMaterial.IsNull())
                fallbackMaterial = CreateFallbackMaterial();

            fallbackMaterial->SetMatrix(0, GetGameObject()->GetLocalToWorldMatrix());
            return fallbackMaterial;
        }
        return material;
    }
    bool Renderer::GetCastShadows() const
    {
        return castShadows;
    }
    bool Renderer::GetReceiveShadows() const
    {
        return receiveShadows;
    }

    void Renderer::SetMaterial(const ObjectPtr<Material>& material)
    {
        this->material = material;
    }
    void Renderer::SetCastShadows(bool castShadows)
    {
        this->castShadows = castShadows;
    }
    void Renderer::SetReceiveShadows(bool receiveShadows)
    {
        this->receiveShadows = receiveShadows;
    }

    void Renderer::Transfer(Transferer& transferer)
    {
        Component::Transfer(transferer);

        TransferFieldInfo(material);
        TransferFieldInfo(castShadows);
        TransferFieldInfo(receiveShadows);
    }
    void Renderer::Enable()
    {
        Component::Enable();

        renderers.push_back(this);
    }
    void Renderer::Disable()
    {
        std::erase(renderers, this);

        Component::Disable();
    }
}
