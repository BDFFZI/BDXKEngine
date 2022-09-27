#pragma once
#include<functional>
#include <utility>

#include "Base/Object/ObjectEvent.h"
#include"Framework/Components/Behavior/BehaviorEvent.h"
#include"Framework/Components/Behavior/Behavior.h"
#include"Framework/Components/Transform.h"
#include"Framework/GameObject.h"

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

        void Export(Exporter& exporter) override;
        void Import(Importer& importer) override;
    };
}
