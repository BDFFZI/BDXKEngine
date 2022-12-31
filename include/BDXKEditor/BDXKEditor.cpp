#include "BDXKEditor.h"
#include <iostream>
#include "BDXKEngine/BDXKEngine.h"
#include "BDXKEngine/Framework/Scene.h"
#include "BDXKEngine/Framework/Behavior/Animator.h"
#include "BDXKEngine/Function/Resources/Resources.h"
#include "BDXKEngine/Function/Time/Time.h"
#include "BDXKEngine/Platform/Serialization/Serialization.h"
#include "BDXKEngine/Platform/GUI/GUI.h"

namespace BDXKEditor
{
    ObjectPtr<SceneWindow> EditorSystem::sceneWindow;
    ObjectPtr<HierarchyWindow> EditorSystem::hierarchyWindow;
    ObjectPtr<InspectorWindow> EditorSystem::inspectorWindow;
    ObjectPtr<ProfilerWindow> EditorSystem::profilerWindow;
    ObjectPtr<ConsoleWindow> EditorSystem::consoleWindow;
    ObjectPtr<ProjectWindow> EditorSystem::projectWindow;
    ObjectPtr<GameWindow> EditorSystem::gameWindow;


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
    void EditorSystem::SetScene(const std::string& sceneName)
    {
        this->sceneName = sceneName;
    }

    void EditorSystem::OnDrawGUI()
    {
        ImGui::ShowDemoWindow();

        if (isRunning)
        {
            ImGui::PushStyleColor(ImGuiCol_MenuBarBg, Color::darkGreen);
            DrawGUI();
            ImGui::PopStyleColor();
        }
        else DrawGUI();
    }
    void EditorSystem::DrawGUI()
    {
        ImGui::BeginMainMenuBar();
        if (isRunning == false)
        {
            if (ImGui::BeginMenu("Scene"))
            {
                if (ImGui::MenuItem("Save"))
                {
                    Scene::Save(sceneName);
                }
                if (ImGui::MenuItem("Load"))
                {
                    Scene::Load(sceneName, true);
                }

                ImGui::EndMenu();
            }
        }

        if (ImGui::BeginMenu("Run"))
        {
            if (isRunning == false && ImGui::MenuItem("Play"))
            {
                SetConstructedObjectEvent({});
                Scene::Save(sceneName);
                Scene::Load(sceneName);
                isRunning = true;
            }
            if (isRunning && ImGui::MenuItem("Stop"))
            {
                SetConstructedObjectEvent([](const Object* object)
                {
                    ObjectGuid::GetOrSetGuid(object->GetInstanceID());
                });
                Scene::Load(sceneName, true);
                isRunning = false;
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
        consoleWindow = EditorWindow::Create<ConsoleWindow>();
        projectWindow = EditorWindow::Create<ProjectWindow>();
        gameWindow = EditorWindow::Create<GameWindow>();

        hierarchyWindow->SetClickGameObjectEvent([](const ObjectPtr<GameObject>& gameObject)
        {
            sceneWindow->SetTarget(gameObject);
            inspectorWindow->SetTarget(gameObject);
        });
        profilerWindow->SetClickObjectEvent([](const ObjectPtrBase& object)
        {
            sceneWindow->SetTarget(object.ToObject<GameObject>());
            inspectorWindow->SetTarget(object);
        });
        projectWindow->SetClickObjectEvent([](const ObjectPtrBase& object)
        {
            inspectorWindow->SetTarget(object);
        });

        hierarchyWindow->Show();
        sceneWindow->Show();
        inspectorWindow->Show();
        profilerWindow->Show();
        consoleWindow->Show();
        projectWindow->Show();
        gameWindow->Show();

        SetConstructedObjectEvent([](const Object* object)
        {
            ObjectGuid::GetOrSetGuid(object->GetInstanceID());
        });

        //两个游戏窗口便于观看，放这里创建是为了测试上方代码的影响
        const ObjectPtr<GameWindow> previewWindow = EditorWindow::Create<GameWindow>();
        previewWindow->SetName("PreviewWindow");
        previewWindow->Show();
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

        ObjectPtr<GameObject> sphere = PresetGameObject::CreateSphere("Small Sphere");
        ObjectPtr<Light> sphere_light;
        {
            sphere->SetParent(aureole);
            sphere->SetLocalScale({0.1f, 0.1f, 0.1f});

            sphere_light = PresetGameObject::CreatePointLight("Red PointLight")->GetComponent<Light>();
            sphere_light->SetType(LightType::Point);
            sphere_light->SetColor(Color::red);
            sphere_light->SetIntensity(0.5f);
            sphere_light->GetGameObject()->SetParent(sphere);

            ObjectPtr<Camera> camera = GameObject::Find("Camera")->GetComponent<Camera>();
            camera->SetClearFlags(ClearFlags::Skybox);
            RenderSettings::SetSkybox(sphere_light->GetShadowMap().ToObjectPtr<TextureCube>());
        }

        ObjectPtr<GameObject> cube = PresetGameObject::CreateCube("Stick");
        {
            cube->SetParent(aureole);
            cube->SetLocalScale({0.2f, 0.05f, 0.05f});
        }
    }

    ObjectPtr<EditorSystem> editorSystem;
    void Run(const std::string& sceneName)
    {
        std::cout.rdbuf(ConsoleWindow::GetConsole().rdbuf());
        BDXKEngine::Run([&]
        {
            if (Resources::IsExisting(sceneName) == false)
            {
                Scene::LoadDefault();
                Scene::Save(sceneName);
            }

            Scene::Load(sceneName, true);
            editorSystem = Object::Create<EditorSystem>();
            editorSystem->SetScene(sceneName);
        });
    }
}
