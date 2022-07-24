#include "Animator.h"

BDXKEngine::Animator::Animator() :Component(L"New Animator")
{
}

void BDXKEngine::Animator::SetAnimation(std::function<void(ObjectPtr<Transform>transfom)> animation)
{
	this->animation = animation;
}

void BDXKEngine::Animator::OnAwake() {
	Component::OnAwake();

	transform = GetGameObject()->GetTransform();
}
void BDXKEngine::Animator::OnUpdate()
{
	if (animation != nullptr)
		animation(transform);
}
