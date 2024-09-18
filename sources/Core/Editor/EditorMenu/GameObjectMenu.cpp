#include "GameObjectMenu.h"
#include "Core/Editor/CoreEditor.h"

namespace BDXKEditor
{
    ObjectPtr<GameObject> GameObjectMenu::NewGameObject(const char* name)
    {
        const ObjectPtr gameObject = CoreEditor::GetInspectorView()->GetTarget().ToObject<GameObject>();
        return gameObject.IsNotNull()
                   ? GameObject::Create(name, gameObject, Scene::GetActiveScene())
                   : GameObject::Create(name, nullptr, Scene::GetActiveScene());
    }
    ObjectPtr<GameObject> GameObjectMenu::CreateGameObject()
    {
        return NewGameObject("New GameObject");
    }

    void GameObjectMenu::ClearParent()
    {
        const ObjectPtr gameObject = CoreEditor::GetInspectorView()->GetTarget().ToObject<GameObject>();
        if (gameObject.IsNotNull())gameObject->SetParent(nullptr);
    }
}
