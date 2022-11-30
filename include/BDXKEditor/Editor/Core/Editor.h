#pragma once
#include "GUITransferer.h"
#include "BDXKEngine/Framework/Core/ScriptableObject.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class Editor
    {
    public:
        static Editor* GetEditor(const Reflective& reflective);
        template <typename TObject, typename TEditor>
        static void SetEditor()
        {
            editors[GetTypeOf<TObject>()] = new TEditor();
        }

        static void AddEditorFallback(const std::function<Editor*(const Reflective&)>& getEditorFallback);

        virtual ~Editor() = default;
        const ObjectPtrBase& GetTarget() const;
        GUITransferer* GetGUITransferer() const;

        void SetTarget(const ObjectPtrBase& target);
        void SetGui(GUITransferer* gui);

        void DrawInspectorGUI();
        void DrawSceneGUI();
    protected:
        virtual void OnInspectorGUI();
        virtual void OnSceneGUI();
    private:
        inline static std::vector<std::function<Editor*(const Reflective&)>> getEditorFallback = {};
        static std::unordered_map<Type, Editor*> editors;

        ObjectPtrBase target = nullptr;
        GUITransferer* gui = nullptr;
    };

    template <typename TObject, typename TEditor>
    struct CustomEditorRegister
    {
        CustomEditorRegister()
        {
            Editor::SetEditor<TObject, TEditor>();
        }
        CustomEditorRegister(std::function<Editor*(const Reflective&)> func)
        {
            Editor::AddEditorFallback(func);
        }
    };

#define CustomEditor(TObject,TEditor) inline CustomEditorRegister<TObject,TEditor> CustomEditor##TObject = {};
#define CustomEditorFallback(func) inline static CustomEditorRegister<void,void> CustomEditorFallback##func = {func};
}
