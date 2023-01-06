#pragma once
#include "BDXKEngine/Base/Object/Pointer/ObjectPtr.h"
#include "BDXKEngine/Function/Graphics/Material.h"

namespace BDXKEngine
{
    //由用户自行填写，所以应将其视作一种辅助功能，而非失败回调，因此要做好回调资源也无效的准备
    class Fallback
    {
    public:
        static const ObjectPtr<Texture2D>& GetTexture2D();
        static const ObjectPtr<Material>& GetMaterial();
        static void SetTexture2D(const ObjectPtr<Texture2D>& texture2D);
        static void SetMaterial(const ObjectPtr<Material>& material);
    private:
        static ObjectPtr<Material> material;
        static ObjectPtr<Texture2D> texture2D;
    };
}
