#pragma once
#include "BDXKEngine/Base/Reflection/Reflection.h"
#include "BDXKEngine/Framework/Core/ScriptableObject.h"
#include "BDXKEngine/Framework/Renderer/RenderEvent.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class EditorWindow : public ScriptableObject, public DrawGUIHandler
    {
    public:
        template <typename TEditorWindow>
        static ObjectPtr<TEditorWindow> Create()
        {
            ObjectPtr window = new TEditorWindow{};
            const Reflection& reflection = Reflection::GetReflection<ScriptableObject>();
            reflection.GetFieldOf<std::string>(window.ToObjectBase(), "name") = GetTypeOf<TEditorWindow>();
            reflection.GetFieldOf<bool>(window.ToObjectBase(), "isEnabling") = false;
            Instantiate(window);
            return window;
        }

        void Show();
    protected:
        virtual void OnGUI();
    private:
        void OnDrawGUI() override;
    };
}
