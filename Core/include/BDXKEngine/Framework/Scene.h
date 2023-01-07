#pragma once
#include "Core/GameObject.h"
#include "Renderer/Core/RenderSettings.h"

namespace BDXKEngine
{
    class Scene : public Object
    {
    public:
        static ObjectPtr<Scene> GetCurrentScene();
        static std::string GetCurrentSceneName();

        static void Create(const std::string& sceneName);
        static void Load(const std::string& sceneName, bool retainPersistent = false);
        static void Save();
        static void SaveAs(const std::string& sceneName);
    private:
        static std::string currentSceneName;

        ObjectPtr<RenderSettings> renderSettings;
        std::vector<ObjectPtr<GameObject>> gameObjects;

        void Transfer(Transferer& transferer) override;
    };

    CustomReflection(Scene)
}
