#pragma once
#include "Core/GameObject.h"
#include "Renderer/Core/RenderSettings.h"

namespace BDXKEngine
{
    class Scene : public Object
    {
    public:
        static void Load(const std::string& sceneName, bool retainPersistent = false);
        static void Save(const std::string& sceneName);
        static ObjectPtr<Scene> GetCurrentScene();
    private:
        ObjectPtr<RenderSettings> renderSettings;
        std::vector<ObjectPtr<GameObject>> gameObjects;

        void Transfer(Transferer& transferer) override;
    };

    CustomReflection(Scene)
}
