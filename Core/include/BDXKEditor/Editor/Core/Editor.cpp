#include "Editor.h"

#include "BDXKEngine/Base/Object/Transferer/ObjectTransferer.h"
#include "imgui/imgui.h"

namespace BDXKEditor
{
    Editor Editor::defaultEditor = {};
    ObjectTransferer<GUITransferer> Editor::defaultGui = {};

    const Editor& Editor::GetEditor(const ObjectPtrBase& target, GUITransferer& gui)
    {
        Editor* editor = nullptr;

        if (const Type type = target->GetType(); editors.contains(type))
            editor = editors[type];
        else
            for (auto& func : getEditorFallback)
                if (editor = func(target); editor != nullptr)break;

        if (editor == nullptr)
            editor = &defaultEditor;

        editor->target = target;
        editor->gui = &gui;
        return *editor;
    }
    void Editor::AddEditorFallback(const std::function<Editor*(const ObjectPtrBase&)>& getEditorFallback)
    {
        Editor::getEditorFallback.push_back(getEditorFallback);
    }

    const ObjectPtrBase& Editor::GetTarget() const
    {
        return target;
    }
    GUITransferer& Editor::GetGUI() const
    {
        if (gui == nullptr)
            throw std::exception("无法获取到GUI工具"); //正常来讲，这个报错不可能触发
        return *gui;
    }

    void Editor::DrawInspectorGUI() const
    {
        OnInspectorGUI();
    }
    void Editor::DrawSceneGUI() const
    {
        OnSceneGUI();
    }
    void Editor::OnInspectorGUI() const
    {
        //类型
        ImGui::TextDisabled(target->GetType().c_str());
        //实例编号
        ImGui::SameLine();
        ImGui::TextDisabled(std::to_string(target->GetInstanceID()).c_str());
        
        target.ToObjectBase()->Transfer(*gui);
    }
    void Editor::OnSceneGUI() const
    {
    }
}
