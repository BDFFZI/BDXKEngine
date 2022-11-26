#pragma once
#include "Core/GameObject.h"
#include "Renderer/Core/RenderSettings.h"

namespace BDXKEngine
{
    struct Scene : Object
    {
        static ObjectPtr<Scene> CreateDefault();
        
        ObjectPtr<RenderSettings> renderSettings;
        std::vector<ObjectPtr<GameObject>> gameObjects;

        void Transfer(Transferer& transferer) override;
    };
}
