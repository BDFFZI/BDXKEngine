#include "ScriptableObject.h"

namespace BDXKEngine
{
    std::unordered_set<ScriptableObject*> ScriptableObject::allScriptableObjects;

    void ScriptableObject::Enable()
    {
        SwitchableObject::Enable();

        allScriptableObjects.insert(this);
    }
    void ScriptableObject::Disable()
    {
        allScriptableObjects.erase(this);

        SwitchableObject::Disable();
    }
}
