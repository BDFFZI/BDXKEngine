#include "Renderer.h"
#include <algorithm>

namespace BDXKEngine
{
    std::vector<Renderer*> Renderer::renderers = {};
    
    const std::vector<Renderer*>& Renderer::GetRendererQueue()
    {
        for (const auto& renderer : renderers)
            if (renderer->GetMaterial().IsNull() || renderer->GetMesh().IsNull())
                throw std::exception("渲染器的材质球或网格为空");

        std::ranges::sort(
            renderers,
            [](const Renderer* a, const Renderer* b)
            {
                return a->GetMaterial()->GetRenderQueue() < b->GetMaterial()->GetRenderQueue();
            }
        );
        return renderers;
    }

    const ObjectPtr<Material>& Renderer::GetMaterial() const
    {
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

    void Renderer::Enable()
    {
        Component::Enable();

        renderers.push_back(this);

        //Debug::LogWarning(static_cast<String>("Renderer::Enable ") + GetInstanceID() + " " + GetName());
    }
    void Renderer::Disable()
    {
        renderers.erase(std::ranges::find(renderers, this));

        Component::Disable();

        //Debug::LogWarning(static_cast<String>("Renderer::Disable ") + GetInstanceID() + " " + GetName());
    }
    void Renderer::Transfer(Transferer& transferer)
    {
        Component::Transfer(transferer);

        TransferFieldInfo(material);
        TransferFieldInfo(castShadows);
        TransferFieldInfo(receiveShadows);
    }
}
