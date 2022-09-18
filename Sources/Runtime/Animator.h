#pragma once
#include<functional>
#include"BehaviorEvent.h"
#include"Behavior.h"
#include"Transform.h"
#include"GameObject.h"

namespace BDXKEngine {
	class Animator :public Behavior, public UpdateHandler
	{
	public:
		void SetAnimation(std::function<void(ObjectPtr<Transform> transfom)> animation);
	private:
		std::function<void(ObjectPtr<Transform> transfom)> animation{};
		ObjectPtr<Transform> transform{};

		void Awake()override;
		void OnUpdate()override;
	};
}
