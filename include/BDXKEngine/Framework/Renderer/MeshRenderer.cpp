#include "MeshRenderer.h"

namespace BDXKEngine
{
    const ObjectPtr<Mesh>& MeshRenderer::GetMesh() const
    {
        return mesh;
    }
    void MeshRenderer::SetMesh(const ObjectPtr<Mesh>& mesh)
    {
        this->mesh = mesh;
    }
}
