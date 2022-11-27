#include "Editor.h"

namespace BDXKEditor
{
    std::unordered_map<Type, Editor*> Editor::editors = {};
    std::function<Editor*(const Type&)> Editor::getEditorFallback = [](const Type&) { return nullptr; };
    Editor* defaultEditor = new Editor();

    Editor* Editor::GetEditor(const Type& type)
    {
        if (editors.contains(type))
            return editors[type];
        if (Editor* editor = getEditorFallback(type); editor != nullptr)
            return editor;

        return defaultEditor;
    }
    void Editor::SetGetEditorFallback(const std::function<Editor*(const Type&)>& getEditorFallback)
    {
        Editor::getEditorFallback = getEditorFallback;
    }
    const ObjectPtrBase& Editor::GetTarget() const
    {
        return target;
    }
    Transferer* Editor::GetGui() const
    {
        return gui;
    }
    void Editor::SetTarget(const ObjectPtrBase& target)
    {
        this->target = target;
    }
    void Editor::SetGui(Transferer* gui)
    {
        this->gui = gui;
    }

    void Editor::DrawInspectorGUI()
    {
        OnInspectorGUI();
    }
    void Editor::DrawSceneGUI()
    {
        OnSceneGUI();
    }
    void Editor::OnInspectorGUI()
    {
        target.ToObjectBase()->Transfer(*gui);
    }
    void Editor::OnSceneGUI()
    {
    }
}
