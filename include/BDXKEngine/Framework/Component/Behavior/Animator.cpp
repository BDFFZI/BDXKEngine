#include "Animator.h"

#include "BDXKEngine/Framework/GameObject.h"


namespace BDXKEngine
{
    void Animator::SetAnimation(const ObjectPtr<AnimatorClip> animatorClip)
    {
        this->animatorClip = animatorClip;
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

        transferrer.TransferField({}, animatorClip);
    }
}
