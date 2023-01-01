#include "Animator.h"

namespace BDXKEngine
{
    ObjectPtr<AnimatorClip> AnimatorClip::Create(const std::function<void(ObjectPtr<GameObject>)>& clip)
    {
        ObjectPtr animatorClip = new AnimatorClip{};

        animatorClip->clip = clip;

        Instantiate(animatorClip);
        return animatorClip;
    }
    const std::function<void(ObjectPtr<GameObject>)>& AnimatorClip::GetClip() const
    {
        return clip;
    }
    void AnimatorClip::SetClip(const std::function<void(ObjectPtr<GameObject>)>& nativeClip)
    {
        this->clip = nativeClip;
    }

    void Animator::SetAnimation(const ObjectPtr<AnimatorClip>& animatorClip)
    {
        this->animatorClip = animatorClip;
    }

    void Animator::OnUpdate()
    {
        if (animatorClip.IsNull() == false)
            animatorClip->GetClip()(GetGameObject());
    }

    void Animator::Transfer(Transferer& transferer)
    {
        Behavior::Transfer(transferer);

        TransferFieldInfo(animatorClip);
    }
}
