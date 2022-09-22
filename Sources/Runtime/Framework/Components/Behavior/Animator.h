#pragma once
#include<functional>

#include "Base/Object/ObjectEvent.h"
#include"Framework/Components/Behavior/BehaviorEvent.h"
#include"Framework/Components/Behavior/Behavior.h"
#include"Framework/Components/Transform.h"
#include"Framework/GameObject.h"

namespace BDXKEngine
{
    class Animator : public Behavior, public AwakeHandler, public UpdateHandler
    {
    public:
        void SetAnimation(std::function<void(ObjectPtr<Transform> transfom)> animation);
    private:
        std::function<void(ObjectPtr<Transform> transfom)> animation{};
        ObjectPtr<Transform> transform{};

        void OnAwake() override;
        void OnUpdate() override;
    };
}
