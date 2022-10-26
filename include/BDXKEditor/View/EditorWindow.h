#pragma once
#include "BDXKEngine/Framework/Component/Renderer/RenderEvent.h"
#include "BDXKEngine/Framework/Object/ScriptableObject.h"
#include "BDXKEditor/System/EditorSystem.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class EditorWindow : public ScriptableObject, public DrawGUIHandler
    {
    public:
        template <typename TWindow>
        static ObjectPtr<TWindow> Create()
        {
            TWindow window;
            window.SetIsEnabling(false);
            return Instantiate(&window);
        }

        void Show();
    protected:
        virtual void OnGUI();
    private:
        void OnDrawGUI() override;
    };
}
