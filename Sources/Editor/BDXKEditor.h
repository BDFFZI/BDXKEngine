#pragma once
#include <BDXKEngine.h>
#include <thread>

#include "CameraController.h"
#include "CreationMenu.h"
#include "MaterialHUD.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class Editor
    {
    public:
        static void Run()
        {
            Engine::Run(Start);

            Debug::Log("\n按任意键退出");
            std::getchar();
        }

    private:
        inline static Window* hierarchyView;
        inline static Window* inspectorView;

        static void Start(const Window& mainWindow)
        {
            CreateDefaultScene();

            hierarchyView = new Window{L"HierarchyView", &mainWindow};
            hierarchyView->Show();
        }

        static void CreateDefaultScene()
        {
            const ObjectPtr<GameObject> camera = CreationMenu::Camera(L"摄像机");
            camera->AddComponent<CameraController>();
            camera->AddComponent<SceneHUD>();
            ObjectPtr<GameObject> sun = CreationMenu::Light::DirectionalLight(L"太阳");
            ObjectPtr<GameObject> ground = CreationMenu::Object3D::Plane(L"地面");
            ObjectPtr<GameObject> sphere = CreationMenu::Object3D::Sphere(L"球");
            //太阳
            {
                ObjectPtr<Transform> transform = sun->GetTransform();
                transform->SetLocalEulerAngles({45, -45, 0});
            }
            //球的展示位置
            {
                ObjectPtr<Transform> transform = sphere->GetTransform();
                transform->SetLocalPosition({0, 0.5f, 0});
            }
            //增加控制功能并调整位置
            {
                ObjectPtr<MaterialHUD> materialHUD = camera->AddComponent<MaterialHUD>();
                materialHUD->materials.push_back(sphere->GetComponent<MeshRenderer>()->GetMaterial());
                camera->GetTransform()->SetLocalPosition({0, 1, -10});
            }

            ObjectPtr<GameObject> blender = CreationMenu::Object3D::CreateObject3D(Resources::GetBlenderMesh(), L"Blender");
            blender->GetTransform()->SetLocalPosition({0, 3.0f, 0});
        }
    };
}
