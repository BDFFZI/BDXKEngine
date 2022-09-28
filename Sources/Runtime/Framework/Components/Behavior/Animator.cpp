#include "Animator.h"
#include <utility>

namespace BDXKEngine
{
    void Animator::SetAnimation(ObjectPtr<AnimatorClip> animatorClip)
    {
        this->animatorClip = std::move(animatorClip);
    }

    void Animator::OnAwake()
    {
        transform = GetGameObject()->GetTransform();
    }
    void Animator::OnUpdate()
    {
        if (animatorClip.IsNull() == false)
            animatorClip->nativeClip(transform);
    }
    void Animator::Transfer(Transferrer& transferrer)
    {
        Behavior::Transfer(transferrer);

        transferrer.TransferObjectPtr({}, animatorClip);
    }
}
