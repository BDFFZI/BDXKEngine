#pragma once
#include"BDXKEngine/Framework/Component/Behavior/BehaviorEvent.h"
#include"BDXKEngine/Framework/Component/Behavior/Behavior.h"
#include"BDXKEngine/Framework/Component/Transform.h"
#include"BDXKEngine/Framework/GameObject.h"
#include "Framework/ScriptableObject/ScriptableObjectEvent.h"

namespace BDXKEngine
{
    class AnimatorClip : public Object
    {
    public:
        static ObjectPtr<AnimatorClip> Create(std::function<void(ObjectPtr<Transform> transfom)> clip)
        {
            AnimatorClip animatorClip{};
            ObjectPtr<AnimatorClip> result = Instantiate(&animatorClip);
            result->nativeClip = std::move(clip);
            return result;
        }

        std::function<void(ObjectPtr<Transform> transfom)> nativeClip{};
    };

    class Animator : public Behavior, public AwakeHandler, public UpdateHandler
    {
    public:
        void SetAnimation(ObjectPtr<AnimatorClip> animatorClip);
    private:
        ObjectPtr<AnimatorClip> animatorClip{};
        ObjectPtr<Transform> transform{};

        void OnAwake() override;
        void OnUpdate() override;

        void Transfer(Transferrer& transferrer) override;
    };
}
