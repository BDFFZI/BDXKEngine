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

    void Animator::Export(Exporter& exporter)
    {
        Behavior::Export(exporter);

        exporter.TransferObjectPtr({}, animatorClip);
    }
    void Animator::Import(Importer& importer)
    {
        Behavior::Import(importer);

        animatorClip = importer.TransferObjectPtr({});
    }
}
