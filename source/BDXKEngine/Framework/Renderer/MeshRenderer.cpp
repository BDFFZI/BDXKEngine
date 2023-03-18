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
    void MeshRenderer::Transfer(Transferer& transferer)
    {
        Renderer::Transfer(transferer);

        TransferFieldInfo(mesh);
    }
}
