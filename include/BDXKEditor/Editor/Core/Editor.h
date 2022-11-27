#pragma once
#include "BDXKEngine/Framework/Core/ScriptableObject.h"


namespace BDXKEditor
{
    using namespace BDXKEngine;

    class Editor
    {
    public:
        static Editor* GetEditor(const Type& type);
        template <typename TObject, typename TEditor>
        static void SetEditor()
        {
            editors[GetTypeOf<TObject>()] = new TEditor();
        }

        static void SetGetEditorFallback(const std::function<Editor*(const Type&)>& getEditorFallback);

        virtual ~Editor() = default;
        const ObjectPtrBase& GetTarget() const;
        Transferer* GetGui() const;

        void SetTarget(const ObjectPtrBase& target);
        void SetGui(Transferer* gui);

        void DrawInspectorGUI();
        void DrawSceneGUI();
    protected:
        virtual void OnInspectorGUI();
        virtual void OnSceneGUI();
    private:
        static std::function<Editor*(const Type&)> getEditorFallback;
        static std::unordered_map<Type, Editor*> editors;

        ObjectPtrBase target = nullptr;
        Transferer* gui = nullptr;
    };

    template <typename TObject, typename TEditor>
    struct CustomEditorLauncher
    {
        CustomEditorLauncher()
        {
            Editor::SetEditor<TObject, TEditor>();
        }
    };

#define CustomEditor(TObject,TEditor) inline CustomEditorLauncher<TObject,TEditor> CustomEditor##TObject = {};
}
