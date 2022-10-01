﻿#include "Renderer.h"
#include <utility>
#include "BDXKEngine/Base/Extension/Vector.h"
#include "BDXKEngine/Framework/Component/Renderer/RenderManager.h"

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

        RenderManager::renderers.emplace_back(this);

        //Debug::LogWarning(static_cast<String>("Renderer::Enable ") + GetInstanceID() + " " + GetName());
    }
    void Renderer::Disable()
    {
        Vector::Remove(RenderManager::renderers, {this});

        Component::Disable();

        //Debug::LogWarning(static_cast<String>("Renderer::Disable ") + GetInstanceID() + " " + GetName());
    }
    void Renderer::Transfer(Transferrer& transferrer)
    {
        Component::Transfer(transferrer);

        transferrer.TransferObjectPtr(nameof(material), material);
        transferrer.TransferObjectPtr(nameof(mesh), mesh);
        transferrer.TransferBool("castShadows", castShadows);
        transferrer.TransferBool("receiveShadows", receiveShadows);
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