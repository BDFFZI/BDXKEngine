#include "RenderSettings.h"

#include "BDXKEngine/Function/Assets/Assets.h"
#include "BDXKEngine/Platform/GL/Shader/ShaderImporter.h"

namespace BDXKEngine
{
    ObjectPtr<RenderSettings> RenderSettings::Create()
    {
        ObjectPtr renderSettings = new RenderSettings();

        //绘制天空盒
        renderSettings->skyboxMaterial = Assets::GetSkyboxMaterial();

        Instantiate(renderSettings);
        return renderSettings;
    }
    void RenderSettings::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

        TransferFieldInfo(skybox);
    }
}
