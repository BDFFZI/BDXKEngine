#pragma once
#include "../Sources/Runtime/BDXKEngine.h"

namespace Assembly {
	using namespace BDXKEngine;
	class AutoDestroy :public Component, public UpdateEvent {
		float time;
		ObjectPtr<Transform> transform;

		void OnAwake()override {
			Component::OnAwake();
			time = Time::GetRealtimeSinceStartup();
			transform = GetTransform();
		}

		void OnUpdate()override {
			transform->SetLocalPosition(transform->GetLocalPosition() + transform->GetFront() * Time::GetDeltaTime() * 3);
			if (Time::GetRealtimeSinceStartup() - time > 5)
				Destroy(GetGameObject());
		}
	};
}