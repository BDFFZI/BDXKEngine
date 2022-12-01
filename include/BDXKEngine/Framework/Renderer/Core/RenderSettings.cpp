#include "RenderSettings.h"

#include "BDXKEngine/Function/Assets/Assets.h"
#include "BDXKEngine/Platform/GL/Shader/ShaderImporter.h"

namespace BDXKEngine
{
    void RenderSettings::Reset()
    {
        ObjectPtr renderSettings = new RenderSettings();
        renderSettings->skyboxMaterial = Assets::GetSkyboxMaterial(); //绘制天空盒
        Instantiate(renderSettings);
    }

    const ObjectPtr<TextureCube>& RenderSettings::GetSkybox() const
    {
        return skybox;
    }
    const ObjectPtr<Material>& RenderSettings::GetSkyboxMaterial() const
    {
        return skyboxMaterial;
    }
    void RenderSettings::SetSkybox(const ObjectPtr<TextureCube>& skybox)
    {
        this->skybox = skybox;
    }
    void RenderSettings::SetSkyboxMaterial(const ObjectPtr<Material>& skyboxMaterial)
    {
        this->skyboxMaterial = skyboxMaterial;
    }

    void RenderSettings::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

        TransferFieldInfo(skybox);
    }
}
