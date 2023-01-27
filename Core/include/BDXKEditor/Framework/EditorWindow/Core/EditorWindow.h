#pragma once
#include "BDXKEngine/Base/Reflection/Reflection.h"
#include "BDXKEngine/Framework/Core/ScriptableObject.h"
#include "BDXKEngine/Framework/Renderer/RenderEvent.h"
#include "BDXKEngine/Platform/Serialization/Serialization.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class EditorWindow : public ScriptableObject, public DrawGUIHandler
    {
    public:
        template <typename TEditorWindow>
        static ObjectPtr<TEditorWindow> Create(bool closeable = true)
        {
            auto& reflection = Reflection::GetReflection<ScriptableObject>();

            ObjectPtr<EditorWindow> window = new TEditorWindow{};
            reflection.GetFieldOf<bool>(window.ToObjectBase(), "isEnabling") = false;

            Serialization::UnMarkPersistent(window.GetInstanceID());
            Instantiate(window);

            const size_t typeWindowCount = Object::FindObjectsOfType<TEditorWindow>().size() - 1;
            window->title = window->GetType() + (typeWindowCount == 0 ? "" : " " + std::to_string(typeWindowCount));
            window->closeable = true;

            return window.ToObjectPtr<TEditorWindow>();
        }

        bool GetCloseable() const;
        void SetCloseable(bool closeable);
        void Show();
    protected:
        virtual bool HasMenu();
        virtual void OnGUI() = 0;
    private:
        static std::vector<ObjectPtr<EditorWindow>> windows; //抑制自动回收

        std::string title;
        bool closeable = false;

        void OnDrawGUI() override;
        void Awake() override;
        void Destroy() override;
    };
}
