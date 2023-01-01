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
    const ObjectPtr<Material>& RenderSettings::GetUnlitMaterial()
    {
        return GetSingleton()->unlitMaterial;
    }
    void RenderSettings::SetSkybox(const ObjectPtr<TextureCube>& skybox)
    {
        GetSingleton()->skybox = skybox;
    }
    void RenderSettings::SetSkyboxMaterial(const ObjectPtr<Material>& skyboxMaterial)
    {
        GetSingleton()->skyboxMaterial = skyboxMaterial;
    }

    void RenderSettings::SetUnlitMaterial(const ObjectPtr<Material>& unlitMaterial)
    {
        GetSingleton()->unlitMaterial = unlitMaterial;
    }
    void RenderSettings::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

        TransferFieldInfo(skybox);
        TransferFieldInfo(skyboxMaterial);
        TransferFieldInfo(unlitMaterial);
    }
}
