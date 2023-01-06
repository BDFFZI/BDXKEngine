#include "Fallback.h"

namespace BDXKEngine
{
    ObjectPtr<Material> Fallback::material = {};
    ObjectPtr<Texture2D> Fallback::texture2D = {};

    const ObjectPtr<Texture2D>& Fallback::GetTexture2D()
    {
        return texture2D;
    }
    const ObjectPtr<Material>& Fallback::GetMaterial()
    {
        return material;
    }
    void Fallback::SetTexture2D(const ObjectPtr<Texture2D>& texture2D)
    {
        Fallback::texture2D = texture2D;
    }
    void Fallback::SetMaterial(const ObjectPtr<Material>& material)
    {
        Fallback::material = material;
    }
}
