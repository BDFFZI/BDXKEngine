#include "RenderSettings.h"
#include "BDXKEngine/Platform/GL/Shader/ShaderImport.h"

namespace BDXKEngine
{
    const ObjectPtr<TextureCube>& RenderSettings::GetSkybox()
    {
        return GetSingleton()->skybox;
    }
    const ObjectPtr<Material>& RenderSettings::GetSkyboxMaterial()
    {
        return GetSingleton()->skyboxMaterial;
    }
    void RenderSettings::SetSkybox(const ObjectPtr<TextureCube>& skybox)
    {
        GetSingleton()->skybox = skybox;
    }
    void RenderSettings::SetSkyboxMaterial(const ObjectPtr<Material>& skyboxMaterial)
    {
        GetSingleton()->skyboxMaterial = skyboxMaterial;
    }

    void RenderSettings::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

        TransferFieldInfo(skybox);
        TransferFieldInfo(skyboxMaterial);
    }
}
