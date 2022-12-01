#include "BDXKEditor.h"
#include <iostream>
#include <ranges>
#include "BDXKEngine/BDXKEngine.h"
#include "BDXKEngine/Framework/Scene.h"
#include "BDXKEngine/Framework/Behavior/Animator.h"
#include "BDXKEngine/Function/Time/Time.h"
#include "BDXKEngine/Platform/Resources/Resources.h"
#include "EditorWindow/Core/EditorWindow.h"
#include "imgui/imgui.h"

namespace BDXKEditor
{
    ObjectPtr<SceneWindow> EditorSystem::sceneWindow;
    ObjectPtr<HierarchyWindow> EditorSystem::hierarchyWindow;
    ObjectPtr<InspectorWindow> EditorSystem::inspectorWindow;
    ObjectPtr<ProfilerWindow> EditorSystem::profilerWindow;


    const ObjectPtr<SceneWindow>& EditorSystem::GetSceneView()
    {
        return sceneWindow;
    }
    const ObjectPtr<HierarchyWindow>& EditorSystem::GetHierarchyView()
    {
        return hierarchyWindow;
    }
    const ObjectPtr<InspectorWindow>& EditorSystem::GetInspectorView()
    {
        return inspectorWindow;
    }
    void EditorSystem::SetScene(const std::string& name)
    {
        sceneFile = name;
    }

    void EditorSystem::OnDrawGUI()
    {
        ImGui::ShowDemoWindow();

        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("Scene"))
        {
            if (ImGui::MenuItem("Save"))
            {
                const ObjectPtr<Scene> scene = Scene::GetCurrentScene();
                Resources::Save(sceneFile, scene, Resources::GetJsonSerializer());
            }
            if (ImGui::MenuItem("Load"))
            {
                // Scene::Load("scene.json");
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    void EditorSystem::OnAwake()
    {
        hierarchyWindow = EditorWindow::Create<HierarchyWindow>();
        sceneWindow = EditorWindow::Create<SceneWindow>();
        inspectorWindow = EditorWindow::Create<InspectorWindow>();
        profilerWindow = EditorWindow::Create<ProfilerWindow>();

        hierarchyWindow->SetClickGameObjectEvent([](const ObjectPtr<GameObject>& gameObject)
        {
            sceneWindow->SetTarget(gameObject);
            inspectorWindow->SetTarget(gameObject);
        });

        hierarchyWindow->Show();
        sceneWindow->Show();
        inspectorWindow->Show();
        profilerWindow->Show();
    }
    void EditorSystem::OnDestroy()
    {
        sceneWindow = nullptr;
        hierarchyWindow = nullptr;
        inspectorWindow = nullptr;
        profilerWindow = nullptr;
    }
    void TestLight()
    {
        ObjectPtr<GameObject> aureole = GameObject::Create("Aureole");
        {
            aureole->SetParent(GameObject::Find("Camera"));
            aureole->SetLocalPosition({0, 0, 0.7f});

            const ObjectPtr<Animator> animator = Component::Create<Animator>(aureole);
            animator->SetAnimation(AnimatorClip::Create([](ObjectPtr<GameObject> transform)
            {
                Vector3 position = transform->GetLocalPosition();
                position.x = std::cosf(Time::GetRealtimeSinceStartup()) * 0.3f;
                position.y = std::sinf(Time::GetRealtimeSinceStartup()) * 0.3f;
                transform->SetLocalPosition(position);

                Vector3 eulerAngles = transform->GetLocalEulerAngles();
                eulerAngles.z -= 90 * Time::GetDeltaTime();
                transform->SetLocalEulerAngles(eulerAngles);
            }));
        }

        ObjectPtr<GameObject> sphere = Creation::CreateSphere("Small Sphere");
        ObjectPtr<Light> sphere_light;
        {
            sphere->SetParent(aureole);
            sphere->SetLocalScale({0.1f, 0.1f, 0.1f});

            sphere_light = Creation::CreatePointLight("Red PointLight")->GetComponent<Light>();
            sphere_light->SetType(LightType::Point);
            sphere_light->SetColor(Color::red);
            sphere_light->SetIntensity(0.5f);
            sphere_light->GetGameObject()->SetParent(sphere);

            ObjectPtr<Camera> camera = GameObject::Find("Camera")->GetComponent<Camera>();
            camera->SetClearFlags(ClearFlags::Skybox);
            RenderSettings::GetSingleton().SetSkybox(sphere_light->GetShadowMap().ToObjectPtr<TextureCube>());
        }

        ObjectPtr<GameObject> cube = Creation::CreateCube("Stick");
        {
            cube->SetParent(aureole);
            cube->SetLocalScale({0.2f, 0.05f, 0.05f});
        }
    }

    ObjectPtr<EditorSystem> editorSystem;
    void Run(const std::string& sceneFile)
    {
        BDXKEngine::Run(sceneFile, [&]
        {
            editorSystem = Object::Create<EditorSystem>();
            editorSystem->SetScene(sceneFile);
        });
        std::cout << "\n按任意键退出" << std::endl;
        std::getchar();
    }
}
