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

    const ObjectPtr<QualitySettings>& Scene::GetQualitySettings() const
    {
        return qualitySettings;
    }
    const ObjectPtr<GraphicsSettings>& Scene::GetGraphicsSettings() const
    {
        return graphicsSettings;
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

        Object::PreAwake(graphicsSettings.ToObjectBase());
        Object::PreAwake(qualitySettings.ToObjectBase());

        for (const auto& value : gameObjects)
            Object::PreAwake(value.ToObjectBase());
    }
    void Scene::PreDestroy()
    {
        const std::vector gameObjects = {this->gameObjects};
        for (const auto& value : gameObjects)
            Object::PreDestroy(value.ToObjectBase());

        Object::PreDestroy(graphicsSettings.ToObjectBase());
        Object::PreDestroy(qualitySettings.ToObjectBase());

        SwitchableObject::PreDestroy();
    }
    void Scene::Transfer(transferer& transferer)
    {
        SwitchableObject::Transfer(transferer);

        transferer.TransferField("qualitySettings", qualitySettings);
        transferer.TransferField("graphicsSettings", graphicsSettings);
        transferer.TransferField("gameObjects", gameObjects);
    }
}
