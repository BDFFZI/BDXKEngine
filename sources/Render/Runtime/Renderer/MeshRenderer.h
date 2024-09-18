#pragma once
#include "Core/Renderer.h"


namespace BDXKEngine
{
    class MeshRenderer : public Renderer
    {
    public:
        const ObjectPtr<Mesh>& GetMesh() const override;
        void SetMesh(const ObjectPtr<Mesh>& mesh);
    protected:
        void Transfer(Transferer& transferer) override;
    private:
        ObjectPtr<Mesh> mesh;
    };

    CustomReflection(MeshRenderer)
}
