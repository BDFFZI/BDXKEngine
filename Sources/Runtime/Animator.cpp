#include "Animator.h"


void BDXKEngine::Animator::SetAnimation(std::function<void(ObjectPtr<Transform>transfom)> animation)
{
	this->animation = animation;
}

void BDXKEngine::Animator::Awake() {
	Component::Awake();

	transform = GetGameObject()->GetTransform();
}
void BDXKEngine::Animator::OnUpdate()
{
	if (animation != nullptr)
		animation(transform);
}
