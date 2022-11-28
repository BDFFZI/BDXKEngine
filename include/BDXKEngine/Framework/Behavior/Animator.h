#pragma once
#include "BDXKEngine/Base/Object/Core/Object.h"
#include "BDXKEngine/Framework/Core/GameObject.h"
#include "Core/Behavior.h"

namespace BDXKEngine
{
    class AnimatorClip : public Object
    {
    public:
        static ObjectPtr<AnimatorClip> Create(const std::function<void(ObjectPtr<GameObject>)>& clip);

        const std::function<void(ObjectPtr<GameObject>)>& GetClip() const;
        void SetClip(const std::function<void(ObjectPtr<GameObject>)>& nativeClip);
    private:
        std::function<void(ObjectPtr<GameObject>)> clip = nullptr;
    };

    class Animator : public Behavior, public UpdateHandler
    {
    public:
        void SetAnimation(const ObjectPtr<AnimatorClip>& animatorClip);

        void Transfer(Transferer& transferer) override;
    private:
        ObjectPtr<AnimatorClip> animatorClip{};

        void OnUpdate() override;
    };
    
    CustomReflection(Animator)
}
