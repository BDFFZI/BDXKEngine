#include "Renderer.h"
#include <utility>
#include "RendererManager.h"
#include "Base/Extension/Vector.h"

namespace BDXKEngine
{
    ObjectPtr<Material> Renderer::GetMaterial()
    {
        return material;
    }

    ObjectPtr<Mesh> Renderer::GetMesh()
    {
        return mesh;
    }

    bool Renderer::GetCastShadows() const
    {
        return castShadows;
    }

    bool Renderer::GetReceiveShadows() const
    {
        return receiveShadows;
    }

    void Renderer::SetMaterial(ObjectPtr<Material> material)
    {
        this->material = std::move(material);
    }

    void Renderer::SetMesh(ObjectPtr<Mesh> mesh)
    {
        this->mesh = std::move(mesh);
    }

    void Renderer::Enable()
    {
        Component::Enable();
        
        RendererManager::renderers.emplace_back(this);
    }

    void Renderer::Disable()
    {
        Vector::Remove(RendererManager::renderers,{this});

        Component::Disable();
    }

    void Renderer::SetCastShadows(bool castShadows)
    {
        this->castShadows = castShadows;
    }

    void Renderer::SetReceiveShadows(bool receiveShadows)
    {
        this->receiveShadows = receiveShadows;
    }
}
