#include "CoreEditor.h"
#include "Core/Editor/Core/Assets/AssetDatabase.h"
#include "Core/Runtime/Base/Data/String/String.h"
#include "Core/Runtime/Base/Object/Serialization/ObjectSerialization.h"
#include "Core/Runtime/Function/GUI/GUI.h"
#include "Core/Editor/EditorMenu/GameObjectMenu.h"
#include "Core/Editor/Core/Settings/Settings.h"

namespace BDXKEditor
{
    void EditorSetting::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);
        TransferFieldInfo(sceneName);
    }

    void CoreEditor::AddClickObjectEvent(const std::function<void(const ObjectPtrBase&)>& clickObjectEvent)
    {
        clickObjectEvents.emplace_back(clickObjectEvent);
    }
    const ObjectPtr<HierarchyWindow>& CoreEditor::GetHierarchyView()
    {
        return hierarchyWindow;
    }
    const ObjectPtr<InspectorWindow>& CoreEditor::GetInspectorView()
    {
        return inspectorWindow;
    }

    void CoreEditor::NewScene()
    {
        Scene::Create("Untitled.scene");
    }
    void CoreEditor::SaveScene()
    {
        const auto scene = Scene::GetActiveScene();
        AssetDatabase::Save("Assets/" + scene->GetName(), scene);
        Scene::Save(scene); //提供给运行时用
    }
    void CoreEditor::Copy()
    {
        copying = inspectorWindow->GetTarget();
    }
    void CoreEditor::Paste()
    {
        if (copying.IsNull()) return;

        if (copying.ToObject<GameObject>() != nullptr)
        {
            ObjectSerialization::Clone(copying);
        }
        else if (ObjectSerialization::IsPersistent(copying))
        {
            const ObjectPtrBase clone = ObjectSerialization::Clone(copying);
            const std::string path = AssetDatabase::GetAssetPath(copying).string();
            const std::string::size_type splitIndex = path.rfind('.'); //没后缀也不支持导入啊，所以百分百能搜到
            const std::string fullName = path.substr(0, splitIndex);
            const std::string extension = path.substr(splitIndex);

            int cloneID = 0;
            std::string clonePath = {};
            do
            {
                cloneID++;
                clonePath = std::format("{} {}{}", fullName, cloneID, extension);
            }
            while (std::filesystem::exists(clonePath) == true);

            AssetDatabase::Save(clonePath, clone);
        }
    }
    void CoreEditor::Play()
    {
        static std::string playScene = {};

        if (isPlaying == false)
        {
            SaveScene();
            playScene = Scene::GetActiveScene()->GetName(); //记录启动场景，用于停止时还原
            //默认Guid为空时加载，这样场景里的东西就不会被识别为持久化物体，即可正常触发事件
            Scene::Load(playScene);
            isPlaying = true;
            EditorMenu::SetMenuBarColor(Color::darkGreen);
        }
        else
        {
            //取消用户的持久化游戏物体
            for (const ObjectPtrBase& object : std::vector{Scene::GetDurableScene()->GetAllObjects()})
            {
                if (object->GetName().starts_with("Editor"))
                    continue; //编辑器的不用管
                if (ObjectPtr gameObject = object.ToObject<GameObject>(); gameObject.IsNotNull())
                    GameObject::UnMarkDurable(gameObject);
            }
            //默认Guid为设为场景Guid，这样场景里的东西就会被识别为和场景一起持久化的物体，也不会再触发事件
            Scene::Load(playScene, AssetImporter::GetAtPath("Assets/" + playScene)->GetTargetGuid());
            isPlaying = false;
            EditorMenu::SetMenuBarColor(Color::lightBlue);
        }
    }

    void CoreEditor::OnLateUpdate()
    {
        ImGui::ShowDemoWindow();

        if (ImGui::BeginMainMenuBar())
        {
            //其他信息
            ImGui::PushStyleColor(ImGuiCol_Text, Color::yellow);
            {
                //复制信息
                if (copying.IsNotNull() && ImGui::MenuItem(("Copying:" + copying->GetType()).c_str()))
                    copying = nullptr;
                //当前场景名字
                static char sceneName[64];
                strcpy_s(sceneName, Scene::GetActiveScene()->GetName().c_str());
                ImGui::InputText("##CurrentSceneName", sceneName, sizeof (sceneName));
                Scene::GetActiveScene()->SetName(sceneName);
            }
            ImGui::PopStyleColor();

            ImGui::EndMainMenuBar();
        }
    }

    void CoreEditor::Initialize(const std::string& sceneName)
    {
        CoreRuntime::AddQuitCondition([] { return false; });

        //加载场景
        if (std::filesystem::exists("Assets/" + sceneName))
        {
            Scene::Load(sceneName, AssetImporter::GetAtPath("Assets/" + sceneName)->GetTargetGuid());
        }
        else
        {
            Scene::Create(sceneName);
            SaveScene();
        }

        //加载各种编辑器功能
        hierarchyWindow = EditorWindow::Create<HierarchyWindow>(false);
        inspectorWindow = EditorWindow::Create<InspectorWindow>(false);
        profilerWindow = EditorWindow::Create<ProfilerWindow>(false);
        consoleWindow = EditorWindow::Create<ConsoleWindow>(false);
        projectWindow = EditorWindow::Create<ProjectWindow>(false);
        hierarchyWindow->SetClickObjectEvent([](const ObjectPtrBase& object)
        {
            for (const auto& event : clickObjectEvents)
                event(object);
            inspectorWindow->SetTarget(object);
        });
        profilerWindow->SetClickObjectEvent([](const ObjectPtrBase& object)
        {
            for (const auto& event : clickObjectEvents)
                event(object);
            inspectorWindow->SetTarget(object);
        });
        projectWindow->SetClickObjectEvent([](const ObjectPtrBase& object)
        {
            for (const auto& event : clickObjectEvents)
                event(object);
            inspectorWindow->SetTarget(object);
        });
        hierarchyWindow->Show();
        inspectorWindow->Show();
        profilerWindow->Show();
        consoleWindow->Show();
        projectWindow->Show();


        editorSystem = Create<CoreEditor>(GameObject::Create("EditorSystem", nullptr, Scene::GetDurableScene()));
    }
    void CoreEditor::OnEngineStart()
    {
        Settings::SetEditorSetting<EditorSetting>();
        EditorMenu::SetMenuBarColor(Color::lightBlue);

        if (EditorSetting::sceneName.empty() == false)
        {
            //运行过，加载上次的场景
            Initialize(EditorSetting::sceneName);
            return;
        }

        //首次运行，加载默认场景
        Initialize("Main.scene");
    }
}
