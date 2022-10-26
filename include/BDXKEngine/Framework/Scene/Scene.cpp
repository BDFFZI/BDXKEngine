#include "Scene.h"
#include "BDXKEngine/Framework/Component/Transform.h"
#include "BDXKEngine/Framework/GameObject/GameObject.h"

namespace BDXKEngine
{
    ObjectPtr<Scene> Scene::Create()
    {
        Scene scene{};
        return Instantiate<Scene>(&scene);
    }

    const ObjectPtr<GraphicsSettings>& Scene::GetGraphicsSettings() const
    {
        return graphicsSettings;
    }
    const ObjectPtr<QualitySettings>& Scene::GetQualitySettings() const
    {
        return qualitySettings;
    }

    ObjectPtr<GameObject> Scene::AddGameObject(const std::string& name)
    {
        GameObject gameObject{};
        gameObject.SetName(name);
        gameObject.scene = this;
        Transform source = {};
        gameObject.components.emplace_back(InstantiateNoAwake<Transform>(&source));

        ObjectPtr result = Instantiate(&gameObject);
        gameObjects.emplace_back(result.ToObjectPtr<GameObject>());
        return result;
    }

    ObjectPtr<GameObject> Scene::Find(const std::string& name)
    {
        const auto& result = std::ranges::find_if(
            gameObjects,
            [&](const ObjectPtr<GameObject>& gameObject) { return gameObject->GetName() == name; }
        );

        return result == gameObjects.end() ? nullptr : *result;
    }

    void Scene::PreAwake()
    {
        SwitchableObject::PreAwake();

        //TODO 零时
        GraphicsSettings graphicsSettingsPrefab{};
        QualitySettings qualitySettingsPrefab{};
        graphicsSettings = InstantiateNoAwake(&graphicsSettingsPrefab);
        qualitySettings = InstantiateNoAwake(&qualitySettingsPrefab);

        Object::Awake(graphicsSettings.ToObjectBase());
        Object::Awake(qualitySettings.ToObjectBase());

        for (const auto& value : gameObjects)
            Object::Awake(value.ToObjectBase());
    }
    void Scene::PreDestroy()
    {
        const std::vector gameObjects = {this->gameObjects};
        for (const auto& value : gameObjects)
            Object::Destroy(value.ToObjectBase());

        Object::Destroy(graphicsSettings.ToObjectBase());
        Object::Destroy(qualitySettings.ToObjectBase());

        SwitchableObject::PreDestroy();
    }
}
