#pragma once
#include "BDXKEditor/EditorWindow/Core/EditorWindow.h"
#include "BDXKEngine/Framework/Core/GameObject.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class HierarchyWindow : public EditorWindow
    {
    public:
        void SetClickGameObjectEvent(const std::function<void(const ObjectPtr<GameObject>&)>& clickGameObjectEvent);
    private:
        std::unordered_map<int, bool> isSpreads;
        std::function<void(const ObjectPtr<GameObject>&)> clickGameObjectEvent;

        void DrawGameObject(const ObjectPtr<GameObject>& gameObject, float offsetX);
        void OnGUI() override;
    };
}
