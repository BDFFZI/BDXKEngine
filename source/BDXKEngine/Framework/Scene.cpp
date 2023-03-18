#include "Scene.h"
#include <filesystem>
#include "BDXKEngine/Function/Resources/Resources.h"
#include "BDXKEngine/Function/Time/Time.h"
#include "BDXKEngine/Platform/Serialization/Serialization.h"
#include "BDXKEngine/Framework/Physics/Collider/SphereCollider.h"
#include "Renderer/Core/Camera.h"

namespace BDXKEngine
{
    std::string Scene::currentSceneName = {};

    ObjectPtr<Scene> Scene::GetCurrentScene()
    {
        ObjectPtr scene = new Scene{};
        Instantiate(scene);
        scene->renderSettings = RenderSettings::GetSingleton();
        scene->physicsSettings = PhysicsSettings::GetSingleton();
        for (const auto& item : GameObject::GetGameObjects())
        {
            if (item->GetParent().IsNull())
                scene->gameObjects.emplace_back(item);
        }
        return scene;
    }
    std::string& Scene::GetCurrentSceneName()
    {
        return currentSceneName;
    }

    void Scene::Create(const std::string& sceneName)
    {
        GameObject::Clear();
        currentSceneName = sceneName;
    }
    void Scene::Load(const std::string& sceneName, bool retainPersistent)
    {
        GameObject::Clear();
        currentSceneName = sceneName;
        const ObjectPtr<Scene> scene = Resources::Load<Scene>(sceneName, retainPersistent);
        Time::RenewFrame();
    }
    void Scene::Save()
    {
        Resources::Save(currentSceneName, GetCurrentScene());
    }
    void Scene::SaveAs(const std::string& sceneName)
    {
        const ObjectPtr<Scene> newScene = GetCurrentScene();
        Resources::Save(sceneName, newScene);
    }

    void Scene::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

        TransferFieldInfo(renderSettings);
        TransferFieldInfo(physicsSettings);
        TransferFieldInfo(gameObjects);
    }
}
