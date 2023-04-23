#pragma once
#include "Core/Runtime/CoreRuntime.h"
#include "Core/Runtime/Base/Reflection/Reflection.h"
#include "Core/Runtime/Base/Object/Serialization/ObjectSerialization.h"
#include "Core/Runtime/Framework/Actor/Behavior.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class EditorWindow : public Behavior, public LateUpdateEvent
    {
    public:
        template <typename TEditorWindow>
        static ObjectPtr<TEditorWindow> Create(const bool closeable = true)
        {
            auto& reflection = Reflection::GetReflection<Behavior>();

            const ObjectPtr<EditorWindow> window = Component::Create<TEditorWindow>(gameObject, false).template ToObjectPtr<EditorWindow>();
            reflection.GetFieldOf<bool>(window.ToObject(), "isEnabling") = false;
            window->closeable = closeable;
            AwakeObject(window);

            return window.ToObjectPtr<TEditorWindow>();
        }

        bool GetCloseable() const;
        void SetCloseable(bool closeable);
        void Show();

    protected:
        virtual bool HasMenu();
        virtual void OnGUI() = 0;

    private:
        inline static ObjectPtr<GameObject> gameObject;

        static void OnEngineStart();
        CustomEngineStartEvent(OnEngineStart, 0)

        bool closeable = false;

        void OnLateUpdate() override;
    };
}
