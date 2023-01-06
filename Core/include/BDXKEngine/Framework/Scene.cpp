#include "Scene.h"
#include "BDXKEngine/Function/Resources/Resources.h"
#include "BDXKEngine/Function/Time/Time.h"
#include "BDXKEngine/Platform/Serialization/Serialization.h"
#include "BDXKEngine/Framework/Physics/Collider/SphereCollider.h"
#include "Renderer/Core/Camera.h"

namespace BDXKEngine
{
    void Scene::Load(const std::string& sceneName, bool retainPersistent)
    {
        GameObject::Clear();
        const ObjectPtr<Scene> scene = Resources::Load<Scene>(sceneName, retainPersistent);
        Time::RenewFrame();
    }
    void Scene::Save(const std::string& sceneName)
    {
        const ObjectPtr<Scene> newScene = GetCurrentScene();
        newScene->SetName(sceneName);
        Resources::Save(sceneName, newScene);
    }
    ObjectPtr<Scene> Scene::GetCurrentScene()
    {
        ObjectPtr scene = new Scene{};
        Instantiate(scene);
        scene->renderSettings = RenderSettings::GetSingleton();
        for (const auto& item : GameObject::GetGameObjects())
        {
            if (item->GetParent().IsNull())
                scene->gameObjects.emplace_back(item);
        }
        return scene;
    }

    void Scene::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

        TransferFieldInfo(renderSettings);
        TransferFieldInfo(gameObjects);
    }
}
