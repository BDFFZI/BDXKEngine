#include "BDXKEditor.h"
#include <iostream>
#include "BDXKEngine/BDXKEngine.h"
#include "BDXKEngine/Framework/Scene.h"
#include "BDXKEngine/Framework/Behavior/Animator.h"
#include "BDXKEngine/Function/Resources/Resources.h"
#include "BDXKEngine/Platform/Serialization/Serialization.h"
#include "BDXKEngine/Platform/GUI/GUI.h"
#include "Function/Assets.h"

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
            Scene::LoadDefault();
            SaveScene(sceneName);
        }

        Assets::Load<Scene>(sceneName, true);
        Scene::Load(Assets::LoadImporter(sceneName)->GetTargetGuid(), keepPersistent);
    }
    void EditorSystem::SaveScene(const std::string& sceneName)
    {
        const auto scene = Scene::GetCurrentScene();
        scene->SetName(sceneName);
        Assets::Save(sceneName, scene);
        Scene::Save(sceneName); //仅提供给运行时用
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
                    SaveScene(sceneName);
                }
                if (ImGui::MenuItem("Load"))
                {
                    LoadScene(sceneName);
                }

                ImGui::EndMenu();
            }
        }

        if (ImGui::BeginMenu("Run"))
        {
            if (isRunning == false && ImGui::MenuItem("Play"))
            {
                isRunning = true;

                SaveScene(sceneName);
                SetConstructedObjectEvent({});
                LoadScene(sceneName, false);
            }
            if (isRunning && ImGui::MenuItem("Stop"))
            {
                isRunning = false;

                SetConstructedObjectEvent([](const Object* object)
                {
                    ObjectGuid::GetOrSetGuid(object->GetInstanceID());
                });
                LoadScene(sceneName);
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
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
