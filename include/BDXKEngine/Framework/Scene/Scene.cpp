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

    void Scene::MarkAwake()
    {
        ScriptableObject::MarkAwake();

        //TODO 零时
        GraphicsSettings graphicsSettingsPrefab{};
        QualitySettings qualitySettingsPrefab{};
        graphicsSettings = InstantiateNoAwake(&graphicsSettingsPrefab);
        qualitySettings = InstantiateNoAwake(&qualitySettingsPrefab);

        BDXKObject::MarkAwake(graphicsSettings.ToObjectBase());
        BDXKObject::MarkAwake(qualitySettings.ToObjectBase());

        for (const auto& value : gameObjects)
            BDXKObject::MarkAwake(value.ToObjectBase());
    }
    void Scene::PreDestroy()
    {
        const std::vector gameObjects = {this->gameObjects};
        for (const auto& value : gameObjects)
            BDXKObject::MarkDestroy(value.ToObjectBase());

        BDXKObject::MarkDestroy(graphicsSettings.ToObjectBase());
        BDXKObject::MarkDestroy(qualitySettings.ToObjectBase());

        ScriptableObject::PreDestroy();
    }
    void Scene::Transfer(transferer& transferer)
    {
        ScriptableObject::Transfer(transferer);

        transferer.TransferField("qualitySettings", qualitySettings);
        transferer.TransferField("graphicsSettings", graphicsSettings);
        transferer.TransferField("gameObjects", gameObjects);
    }
}
