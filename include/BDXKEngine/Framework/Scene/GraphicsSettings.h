#pragma once
#include "BDXKEngine/Base/Object/ObjectPtr.h"
#include "BDXKEngine/Platform/GL/Resources/TextureCube.h"
#include "BDXKEngine/Framework/Object/ScriptableObject.h"

namespace BDXKEngine
{
    class GraphicsSettings : public ScriptableObject
    {
    public:
        const ObjectPtr<TextureCube>& GetSkybox() const;
        void SetSkybox(const ObjectPtr<TextureCube>& skybox);
    private:
        ObjectPtr<TextureCube> skybox = nullptr;
    };
}
