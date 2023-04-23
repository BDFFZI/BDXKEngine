#pragma once
#include "Core/Editor/Core/Editor/EditorWindow.h"
#include "Core/Runtime/Framework/Actor/GameObject.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    class HierarchyWindow : public EditorWindow
    {
    public:
        void SetClickObjectEvent(const std::function<void(const ObjectPtrBase&)>& clickGameObjectEvent);

    private:
        std::function<void(const ObjectPtrBase&)> clickObjectEvent;
        std::unordered_map<ID, bool> isUnfolding;

        void DrawGameObject(const ObjectPtr<GameObject>& gameObject, float offsetX);
        void DrawObject(const ObjectPtrBase& object) const;
        void DrawScene(const ObjectPtr<Scene>& scene);
        void OnGUI() override;
    };
    CustomReflection(HierarchyWindow)
}
