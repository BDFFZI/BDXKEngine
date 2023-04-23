#include "RenderSetting.h"

namespace BDXKEngine
{
    const ObjectPtr<Material>& RenderSetting::GetSkybox()
    {
        return skybox;
    }
    void RenderSetting::SetSkybox(const ObjectPtr<Material>& skybox)
    {
        RenderSetting::skybox = skybox;
    }

    void RenderSetting::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

        TransferFieldInfo(skybox);
    }
}
