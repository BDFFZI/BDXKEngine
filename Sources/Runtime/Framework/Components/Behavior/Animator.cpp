#include "Animator.h"
#include <utility>

namespace BDXKEngine
{
    void Animator::SetAnimation(std::function<void(ObjectPtr<Transform> transfom)> animation)
    {
        this->animation = std::move(animation);
    }

    void Animator::OnAwake()
    {
        transform = GetGameObject()->GetTransform();
    }

    void Animator::OnUpdate()
    {
        if (animation != nullptr)
            animation(transform);
    }
}
