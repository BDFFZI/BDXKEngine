#include "Editor.h"

#include "Core/Runtime/Base/Debug/Debug.h"
#include "Core/Runtime/Base/Object/Transferer/ObjectTransferer.h"
#include "Core/Runtime/Function/GUI/GUI.h"

namespace BDXKEditor
{
    ObjectTransferer<GUITransferer> Editor::defaultGui = {};

    std::unique_ptr<Editor> Editor::GetEditorDefault(const ObjectPtrBase& target, GUITransferer& gui)
    {
        std::unique_ptr<Editor> editor = std::unique_ptr<Editor>{new Editor{}};
        editor->target = target;
        editor->gui = &gui;
        return editor;
    }
    std::unique_ptr<Editor> Editor::GetEditor(const ObjectPtrBase& target, GUITransferer& gui)
    {
        Editor* editor = nullptr;

        if (const Type type = target->GetType(); editors.contains(type))
            editor = editors[type]();
        else
            for (auto& func : getEditorFallback)
                if (editor = func(target); editor != nullptr)break;

        if (editor == nullptr)
            return GetEditorDefault(target, gui);

        editor->target = target;
        editor->gui = &gui;
        return std::unique_ptr<Editor>{editor};
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
        ImGui::Button(std::to_string(target->GetObjectID()).c_str());
        GUI::IsDragSource(target);

        target.ToObject()->Transfer(*gui);
    }
    void Editor::OnSceneGUI() const
    {
    }
}
