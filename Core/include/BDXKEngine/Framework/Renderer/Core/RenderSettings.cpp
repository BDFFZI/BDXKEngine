#include "RenderSettings.h"

namespace BDXKEngine
{
    const ObjectPtr<Material>& RenderSettings::GetSkybox()
    {
        return GetSingleton()->skybox;
    }
    const ObjectPtr<Material>& RenderSettings::GetBlitMaterial()
    {
        return GetSingleton()->blitMaterial;
    }
    void RenderSettings::SetSkybox(const ObjectPtr<Material>& skybox)
    {
        GetSingleton()->skybox = skybox;
    }
    void RenderSettings::SetBlitMaterial(const ObjectPtr<Material>& blitMaterial)
    {
        GetSingleton()->blitMaterial = blitMaterial;
    }

    void RenderSettings::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

        TransferFieldInfo(skybox);
        TransferFieldInfo(blitMaterial);
    }
}
