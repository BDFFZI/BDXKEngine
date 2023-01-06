#include "BDXKEditor.h"
#include <iostream>
#include "BDXKEngine/BDXKEngine.h"
#include "BDXKEngine/Framework/Scene.h"
#include "BDXKEngine/Framework/Behavior/Animator.h"
#include "BDXKEngine/Function/Fallback/Fallback.h"
#include "BDXKEngine/Platform/Serialization/Serialization.h"
#include "BDXKEngine/Platform/GUI/GUI.h"
#include "Function/Assets.h"
#include "Function/AssetsBuiltIn.h"
#include "Function/Debug.h"
#include "Menu/CreateAssetMenu.h"
#include "Menu/CreateGameObjectMenu.h"

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
        LoadScene(sceneName);
    }

    void EditorSystem::LoadScene(const std::string& sceneName, bool keepPersistent)
    {
        if (Assets::IsExisting(sceneName) == false)
        {
            GameObject::Clear();
            RenderSettings::SetSkybox(CreateAssetMenu::CreateSkyboxMaterial());
            RenderSettings::SetBlitMaterial(CreateAssetMenu::CreateUnlitMaterial());
            const ObjectPtr<GameObject> sun = CreateGameObjectMenu::CreateDirectionalLight();
            sun->SetName("Sun");
            sun->SetLocalEulerAngles({45, -45, 0});
            const ObjectPtr<GameObject> ground = CreateGameObjectMenu::CreatePlane();
            ground->SetName("Ground");
            const ObjectPtr<GameObject> sphere = CreateGameObjectMenu::CreateSphere();
            sphere->SetName("Sphere");
            sphere->SetLocalPosition({0, 0.5f, 0});
            const ObjectPtr<GameObject> camera = CreateGameObjectMenu::CreateCamera();
            camera->SetName("Camera");
            camera->SetLocalPosition({0, 1, -10});
            SaveScene(sceneName);
        }

        auto scene = Assets::Load<Scene>(sceneName, true);
        Scene::Load(Assets::LoadImporter(sceneName)->GetTargetGuid(), keepPersistent);
    }
    void EditorSystem::SaveScene(const std::string& sceneName)
    {
        auto scene = Scene::GetCurrentScene();
        scene->SetName(sceneName);
        Assets::Save(sceneName, scene);
        Scene::Save(sceneName); //仅提供给运行时用
    }

    void EditorSystem::DrawMainMenuBar()
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save"))
            {
                SaveScene(sceneName);
            }
            if (ImGui::MenuItem("Load"))
            {
                LoadScene(sceneName);
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (isRunning == false && ImGui::MenuItem("Play"))
            {
                isRunning = true;
                SaveScene(sceneName);

                SetConstructedObjectEvent({});
                Scene::Load(Assets::LoadImporter(sceneName)->GetTargetGuid(), false);
            }
            if (isRunning && ImGui::MenuItem("Stop"))
            {
                isRunning = false;

                SetConstructedObjectEvent([](const Object* object)
                {
                    Serialization::MarkPersistent(object->GetInstanceID());
                });
                Scene::Load(Assets::LoadImporter(sceneName)->GetTargetGuid(), true);
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Assets"))
        {
            for (auto& [name,func] : CreateAssetMenu::GetMenuItems())
            {
                if (ImGui::MenuItem(name.c_str()))
                {
                    ObjectPtrBase asset = func();
                    std::string assetName = CreateAssetMenu::GetAssetName(name);
                    Assets::Save(assetName, asset);
                    Debug::Log("Create Asset " + assetName);
                }
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("GameObject"))
        {
            for (auto& [name,func] : CreateGameObjectMenu::GetMenuItems())
            {
                if (ImGui::MenuItem(name.c_str()))
                {
                    const ObjectPtr<GameObject> gameObject = func();
                    Debug::Log("Create GameObject " + gameObject->GetName());
                }
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Scene"))
        {
            if (ImGui::MenuItem("RenderSettings"))
                inspectorWindow->SetTarget(RenderSettings::GetSingleton());

            ImGui::EndMenu();
        }
    }
    void EditorSystem::OnDrawGUI()
    {
        ImGui::ShowDemoWindow();

        if (isRunning)
        {
            ImGui::PushStyleColor(ImGuiCol_MenuBarBg, Color::darkGreen);
            if (ImGui::BeginMainMenuBar())
            {
                DrawMainMenuBar();
                ImGui::EndMainMenuBar();
            }
            ImGui::PopStyleColor();
        }
        else
        {
            if (ImGui::BeginMainMenuBar())
            {
                DrawMainMenuBar();
                ImGui::EndMainMenuBar();
            }
        }
    }
    void EditorSystem::OnAwake()
    {
        AssetsBuiltIn::Initialize();

        //TODO Fallback需要保存
        const auto fallbackMaterial = CreateAssetMenu::CreateUnlitMaterial();
        const auto fallbackTexture2D = Texture2D::Create(Color::white);
        fallbackMaterial->SetTexture2D(0, fallbackTexture2D);
        fallbackMaterial->SetVector(0, {1, 0, 1, 1});
        Fallback::SetMaterial(fallbackMaterial);
        Fallback::SetTexture2D(fallbackTexture2D);

        //初始化编辑器窗口
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

        //两个游戏窗口便于观看
        const ObjectPtr<GameWindow> previewWindow = EditorWindow::Create<GameWindow>();
        previewWindow->SetName("PreviewWindow");
        previewWindow->Show();

        //非运行模式下创建的物体都是标记持久化的物体
        SetConstructedObjectEvent([](const Object* object)
        {
            Serialization::MarkPersistent(object->GetInstanceID());
        });
    }

    ObjectPtr<EditorSystem> editorSystem;
    void Run(const std::string& sceneName)
    {
        std::cout.rdbuf(ConsoleWindow::GetConsole().rdbuf());
        BDXKEngine::Run([&]
        {
            editorSystem = Object::Create<EditorSystem>();
            editorSystem->SetScene(sceneName);
        });
    }
}
