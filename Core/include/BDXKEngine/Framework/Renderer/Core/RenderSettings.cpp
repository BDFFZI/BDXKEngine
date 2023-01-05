#include "RenderSettings.h"
#include "BDXKEngine/Platform/GL/Shader/ShaderImport.h"

namespace BDXKEngine
{
    const ObjectPtr<Material>& RenderSettings::GetSkybox()
    {
        return GetSingleton()->skybox;
    }
    void RenderSettings::SetSkybox(const ObjectPtr<Material>& skybox)
    {
        GetSingleton()->skybox = skybox;
    }
    
    void RenderSettings::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

        TransferFieldInfo(skybox);
    }
}
