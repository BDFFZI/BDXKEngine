#include "Scene.h"
#include <filesystem>
#include "SceneLibrary.h"
#include "Core/Runtime/Base/Object/Serialization/ObjectSerialization.h"

namespace BDXKEngine
{
    void Scene::AddLoadSceneEvent(const std::function<void(const ObjectPtr<Scene>&)>& event)
    {
        loadSceneEvents.emplace_back(event);
    }
    ObjectPtr<Scene> Scene::GetActiveScene()
    {
        return activeScene;
    }
    ObjectPtr<Scene> Scene::GetDurableScene()
    {
        return durableScene;
    }

    bool Scene::IsExisting(const std::string& sceneName)
    {
        return std::filesystem::exists(SceneLibrary::ParsePath(sceneName));
    }
    void Scene::Create(const std::string& sceneName, const xg::Guid& persistent)
    {
        UnLoad();

        activeScene = Object::Create<Scene>();
        activeScene->SetName(sceneName);
        if (persistent.isValid())
            ObjectSerialization::MarkPersistent(activeScene, persistent);

        for (const auto& loadSceneEvent : loadSceneEvents)
            loadSceneEvent(activeScene);
    }

    void Scene::Load(const std::string& sceneName, const xg::Guid& persistent)
    {
        if (IsExisting(sceneName) == false)
        {
            Debug::LogError("不存在目标场景");
            return;
        }

        UnLoad();

        auto serializer = ObjectSerialization::CreateBinarySerializer();
        activeScene = ObjectSerialization::Load<Scene>(
            SceneLibrary::ParsePath(sceneName), serializer,
            persistent, true);

        for (const auto& loadSceneEvent : loadSceneEvents)
            loadSceneEvent(activeScene);
    }
    void Scene::UnLoad()
    {
        DestroyObject(activeScene);
    }
    void Scene::Save(const ObjectPtr<Scene>& scene)
    {
        if (scene.IsNull())
        {
            Debug::LogWarning("当前没有加载任何场景");
            return;
        }

        auto serializer = ObjectSerialization::CreateBinarySerializer();
        ObjectSerialization::Save(
            SceneLibrary::ParsePath(scene->GetName()), scene, serializer,
            xg::Guid{});
    }

    const std::vector<ObjectPtrBase>& Scene::GetAllObjects()
    {
        return objects;
    }
    void Scene::AddObject(const ObjectPtrBase& objectPtr, const bool first)
    {
        if (first)
            objects.insert(objects.begin(), objectPtr);
        else
            objects.emplace_back(objectPtr);
    }
    void Scene::RemoveObject(const ObjectPtrBase& objectPtr)
    {
        std::erase(objects, objectPtr);
    }
    bool Scene::HaveObject(const ObjectPtrBase& objectPtr)
    {
        return std::ranges::find(objects, objectPtr) != objects.end();
    }

    xg::Guid Scene::NameToGuid(const std::string& name)
    {
        char guid[16];
        memcpy(guid, &name[name.length() - 8], 8); //降低重复概率
        const size_t hash = std::hash<std::string>()(name); //可能重复，但几率很低，先不管了
        *reinterpret_cast<size_t*>(&guid[8]) = hash;
        return xg::Guid(guid);
    }
    void Scene::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

        TransferFieldInfo(objects);
    }
    void Scene::Destroy()
    {
        for (const auto& child : std::vector{objects})
        {
            DestroyObject(child);
        }

        Object::Destroy();
    }
}
