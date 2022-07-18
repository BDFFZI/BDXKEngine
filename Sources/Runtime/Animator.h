#pragma once
#include<functional>
#include"Component.h"
#include"Transform.h"
#include"GameObject.h"

namespace BDXKEngine {
	class Animator :public Component, public LateUpdateEvent
	{
	public:
		void SetAnimation(std::function<void(ObjectPtr<Transform> transfom)> animation)
		{
			this->animation = animation;
		}
	private:
		std::function<void(ObjectPtr<Transform> transfom)> animation{};
		ObjectPtr<Transform> transform{};

		void OnAwake()override {
			Component::OnAwake();

			transform = GetGameObject()->GetTransform();
		}

		void OnLateUpdate()override
		{
			animation(transform);
		}
	};
}
