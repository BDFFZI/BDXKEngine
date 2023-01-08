#pragma once
#include "GUITransferer.h"
#include "BDXKEngine/Base/Object/Transferer/ObjectTransferer.h"
#include "BDXKEngine/Framework/Core/ScriptableObject.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class Editor
    {
    public:
        static const Editor& GetEditor(const ObjectPtrBase& target, GUITransferer& gui = defaultGui);
        template <typename TObject, typename TEditor>
        static void SetEditor()
        {
            editors[GetTypeOf<TObject>()] = new TEditor();
        }
        static void AddEditorFallback(const std::function<Editor*(const ObjectPtrBase&)>& getEditorFallback);

        virtual ~Editor() = default;
        const ObjectPtrBase& GetTarget() const;
        GUITransferer& GetGUI() const;
        void DrawInspectorGUI() const;
        void DrawSceneGUI() const;
    protected:
        virtual void OnInspectorGUI() const;
        virtual void OnSceneGUI() const;
    private:
        static Editor defaultEditor;
        static ObjectTransferer<GUITransferer> defaultGui;
        inline static std::unordered_map<Type, Editor*> editors = {};
        inline static std::vector<std::function<Editor*(const ObjectPtrBase&)>> getEditorFallback = {};

        ObjectPtrBase target = nullptr;
        GUITransferer* gui = &defaultGui;
    };

    template <typename TObject, typename TEditor>
    struct CustomEditorRegister
    {
        CustomEditorRegister()
        {
            Editor::SetEditor<TObject, TEditor>();
        }
    };

#define CustomEditor(TObject,TEditor) inline CustomEditorRegister<TObject,TEditor> CustomEditor##TObject##TEditor = {};
}
