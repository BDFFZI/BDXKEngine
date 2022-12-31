#include "PhysicsEvent.h"

#include "BDXKEngine/Base/Object/Core/ObjectPtr.h"
#include "BDXKEngine/Framework/Core/ScriptableObject.h"
#include "BDXKEngine/Function/Time/Time.h"
#include "BDXKEngine/Platform/Phys/Phys.h"

namespace BDXKEngine
{
    float PhysicsEvent::fixedTime = 0.02f;
    float PhysicsEvent::fixedTimeDelta = 0;

    void PhysicsEvent::Initialize(Window* window)
    {
        window->AddRenewEvent([]
        {
            const std::vector<ObjectPtr<ScriptableObject>> drawGUIHandlers =
                ScriptableObject::FindScriptableObjectsOfType<FixedUpdateHandler, ScriptableObject>();

            fixedTimeDelta += Time::GetDeltaTime();
            while (fixedTimeDelta > fixedTime)
            {
                for (const auto& drawGUIHandler : drawGUIHandlers)
                    if (drawGUIHandler.IsNotNull() && drawGUIHandler->IsNotResource()) drawGUIHandler.ToObject<FixedUpdateHandler>()->OnFixedUpdate();
                Phys::GetScene().simulate(fixedTime);
                Phys::GetScene().fetchResults(true);
                fixedTimeDelta -= fixedTime;
            }
        });
    }
}
