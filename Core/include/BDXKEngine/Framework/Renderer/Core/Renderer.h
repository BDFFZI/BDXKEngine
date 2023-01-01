#pragma once
#include "BDXKEngine/Framework/Core/Component.h"
#include "BDXKEngine/Function/Graphics/Material.h"
#include "BDXKEngine/Platform/GL/Mesh/Mesh.h"

namespace BDXKEngine
{
    class Renderer : public Component
    {
    public:
        static const std::vector<Renderer*>& GetRendererQueue();

        const ObjectPtr<Material>& GetMaterial() const;
        virtual const ObjectPtr<Mesh>& GetMesh() const = 0;
        bool GetCastShadows() const;
        bool GetReceiveShadows() const;

        void SetMaterial(const ObjectPtr<Material>& material);
        void SetCastShadows(bool castShadows);
        void SetReceiveShadows(bool receiveShadows);
    protected:
        void Transfer(Transferer& transferer) override;
        void Awake() override;
        void Enable() override;
        void Disable() override;
    private:
        static std::vector<Renderer*> renderers;

        ObjectPtr<Material> material = nullptr;
        bool castShadows = true;
        bool receiveShadows = true;
    };
}
