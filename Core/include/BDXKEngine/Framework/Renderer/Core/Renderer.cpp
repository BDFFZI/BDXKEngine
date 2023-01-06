#include "Renderer.h"
#include <algorithm>
#include "BDXKEngine/Function/Fallback/Fallback.h"

namespace BDXKEngine
{
    std::vector<Renderer*> Renderer::renderers = {};
    std::vector<Renderer*> rendererQueue = {};

    const std::vector<Renderer*>& Renderer::GetRendererQueue()
    {
        rendererQueue.clear();

        if (Fallback::GetMaterial().IsNotNull())
        {
            for (const auto& renderer : renderers)
                if (renderer->GetMesh().IsNotNull())
                    rendererQueue.emplace_back(renderer);
        }
        else
        {
            for (const auto& renderer : renderers)
                if (renderer->GetMesh().IsNotNull() && renderer->GetMaterial().IsNotNull())
                    rendererQueue.emplace_back(renderer);
        }

        std::ranges::sort(
            rendererQueue,
            [](const Renderer* a, const Renderer* b)
            {
                ObjectPtr<Material> materialA = a->GetMaterial();
                ObjectPtr<Material> materialB = b->GetMaterial();

                if (materialA.IsNull())materialA = Fallback::GetMaterial();
                if (materialB.IsNull())materialB = Fallback::GetMaterial();

                return materialA->GetRenderQueue() < materialB->GetRenderQueue();
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
            Fallback::GetMaterial()->SetMatrix(0, GetGameObject()->GetLocalToWorldMatrix());
            return Fallback::GetMaterial();
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
