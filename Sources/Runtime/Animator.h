#pragma once
#include<functional>
#include"BehaviorEvent.h"
#include"Component.h"
#include"Transform.h"
#include"GameObject.h"

namespace BDXKEngine {
	class Animator :public Component, public UpdateHandler
	{
	public:
		Animator();

		void SetAnimation(std::function<void(ObjectPtr<Transform> transfom)> animation);
	private:
		std::function<void(ObjectPtr<Transform> transfom)> animation{};
		ObjectPtr<Transform> transform{};

		void OnAwake()override;
		void OnUpdate()override;
	};
}
