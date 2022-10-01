#include "ScriptableObject.h"
#include "ScriptableObjectEvent.h"

namespace BDXKEngine
{
    std::unordered_set<ScriptableObject*> ScriptableObject::allScriptableObjects;

    bool ScriptableObject::GetIsActivating() const
    {
        return true;
    }
    void ScriptableObject::SetIsEnabling(bool state)
    {
        if (isEnabling == state)
            return;


        if (GetIsRunning())
        {
            if (GetIsActivating())
            {
                if (state)
                    Enable();
                else
                    Disable();
            }
        }

        isEnabling = state;
    }
    void ScriptableObject::OnUpdateActivating(bool state)
    {
        if (isEnabling)
        {
            if (state)
                Enable();
            else
                Disable();
        }
    }

    void ScriptableObject::Enable()
    {
        if (isAwakened == false)
        {
            if (const auto handler = dynamic_cast<AwakeHandler*>(this); handler != nullptr)
                handler->OnAwake();
            isAwakened = true;
        }

        allScriptableObjects.insert(this);
        if (const auto handler = dynamic_cast<EnableHandler*>(this); handler != nullptr)
            handler->OnEnable();

        Debug::LogWarning("ScriptableObject::Enable " + std::to_string(GetInstanceID()) + " " + GetName());
    }

    void ScriptableObject::Disable()
    {
        allScriptableObjects.erase(this);
        if (const auto handler = dynamic_cast<DisableHandler*>(this); handler != nullptr)
            handler->OnDisable();

        Debug::LogWarning("ScriptableObject::Disable " + std::to_string(GetInstanceID()) + " " + GetName());
    }

    void ScriptableObject::Destroy()
    {
        if (isAwakened)
        {
            if (const auto handler = dynamic_cast<DestroyHandler*>(this); handler != nullptr)
                handler->OnDestroy();
        }

        SwitchableObject::Destroy();
    }
}
