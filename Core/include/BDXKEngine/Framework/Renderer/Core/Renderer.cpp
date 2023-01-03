#include "Renderer.h"
#include <algorithm>

#include "BDXKEngine/Function/Resources/Resources.h"
#include "BDXKEngine/Function/Resources/ResourcesDefault.h"

namespace BDXKEngine
{
    std::vector<Renderer*> Renderer::renderers = {};
    std::vector<Renderer*> rendererQueue = {};

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
                return a->GetMaterialFallback()->GetRenderQueue() < b->GetMaterialFallback()->GetRenderQueue();
            }
        );
        return rendererQueue;
    }
    const ObjectPtr<Material>& Renderer::GetMaterialFallback() const
    {
        if (material.IsNotNull())
        {
            material->SetMatrix(0, GetGameObject()->GetLocalToWorldMatrix());
            return material;
        }

        auto& fallbackMaterial = ResourcesDefault::GetFallbackMaterial();
        fallbackMaterial->SetMatrix(0, GetGameObject()->GetLocalToWorldMatrix());
        return fallbackMaterial;
    }

    const ObjectPtr<Material>& Renderer::GetMaterial() const
    {
        if (material.IsNotNull())
            material->SetMatrix(0, GetGameObject()->GetLocalToWorldMatrix());
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
