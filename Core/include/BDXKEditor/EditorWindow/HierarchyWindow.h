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
        std::function<void(const ObjectPtr<GameObject>&)> clickGameObjectEvent;
        std::unordered_map<int, bool> isUnfolding;

        void DrawGameObject(const ObjectPtr<GameObject>& gameObject, float offsetX);
        void OnGUI() override;
    };
}
