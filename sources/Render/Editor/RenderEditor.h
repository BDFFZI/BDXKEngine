#pragma once
#include "Core/Runtime/CoreRuntime.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;
    class RenderEditor
    {
        static void OnEngineStart();
        static void OnEngineUpdate();
        CustomEngineStartEvent(OnEngineStart, 0)
        CustomEngineUpdateEvent(OnEngineUpdate, 20)
    };

    //创建默认场景
    // AssetsBuiltIn::Initialize();
    // Scene::Create(sceneName);
    // RenderSettings::SetSkybox(CreateAssetMenu::CreateSkyboxMaterial());
    // const ObjectPtr<GameObject> sun = GameObjectMenu::CreateDirectionalLight();
    // sun->SetName("Sun");
    // sun->SetLocalEulerAngles({45, -45, 0});
    // const ObjectPtr<GameObject> ground = GameObjectMenu::CreatePlane();
    // ground->SetName("Ground");
    // const ObjectPtr<GameObject> sphere = GameObjectMenu::CreateSphere();
    // sphere->SetName("Sphere");
    // sphere->SetLocalPosition({0, 0.5f, 0});
    // const ObjectPtr<GameObject> camera = GameObjectMenu::CreateCamera();
    // camera->SetName("Camera");
    // camera->SetLocalPosition({0, 1, -10});
    // SaveScene();
}
