#include "GraphicsSettings.h"

namespace BDXKEngine
{
    const ObjectPtr<TextureCube>& GraphicsSettings::GetSkybox() const
    {
        return skybox;
    }
    void GraphicsSettings::SetSkybox(const ObjectPtr<TextureCube>& skybox)
    {
        this->skybox = skybox;
    }
}
