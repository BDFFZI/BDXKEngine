#pragma once
#include "Core/Runtime/Framework/Actor/Component.h"
#include "Render/Runtime/Graphics/Material.h"
#include "Render/Runtime/Graphics/Mesh.h"

namespace BDXKEngine
{
    class Renderer : public Component
    {
    public:
        static const std::vector<Renderer*>& GetRendererQueue();

        const ObjectPtr<Material>& GetMaterial(bool fallback = false) const;
        virtual const ObjectPtr<Mesh>& GetMesh() const = 0;
        bool GetCastShadows() const;
        bool GetReceiveShadows() const;

        void SetMaterial(const ObjectPtr<Material>& material);
        void SetCastShadows(bool castShadows);
        void SetReceiveShadows(bool receiveShadows);
    protected:
        void Enable() override;
        void Disable() override;
        
        void Transfer(Transferer& transferer) override;
    private:
        static std::vector<Renderer*> renderers;
        static ObjectPtr<Material> fallbackMaterial;

        bool castShadows = true;
        bool receiveShadows = true;
        ObjectPtr<Material> material = nullptr;
    };
}
