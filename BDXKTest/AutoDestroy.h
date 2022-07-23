#pragma once
#include "../Sources/Runtime/BDXKEngine.h"

namespace Assembly {
	using namespace BDXKEngine;
	class AutoDestroy :public Component, public UpdateEvent {
		float time;
		ObjectPtr<Animator> animator;
		ObjectPtr<MeshRenderer> meshRenderer;

		void OnAwake()override {
			Component::OnAwake();

			time = Time::GetRealtimeSinceStartup();
			meshRenderer = GetGameObject()->GetComponent<MeshRenderer>();
			animator = GetGameObject()->AddComponent<Animator>();

			animator->SetAnimation([](ObjectPtr<Transform> transform) {
				transform->SetLocalPosition(transform->GetLocalPosition() + transform->GetFront() * Time::GetDeltaTime() * 3);
				});
		}

		void OnUpdate()override {
			if (Time::GetRealtimeSinceStartup() - time > 2)
				Destroy(animator.GetPtr());
			if (Time::GetRealtimeSinceStartup() - time > 4)
			{
				ObjectPtr<GameObject> gameObject = GetGameObject();
				Destroy(this);
				Destroy(gameObject.GetPtr());
			}
			if ((int)std::fmodf(Time::GetFrameCount() - time, 100) == 0)
			{
				Debug::Log(L"Ëæ»ú:" + Random::ColorHSV().ToString());
				Color color = Random::ColorHSV();
				color.a = 0.3f;

				meshRenderer->GetMaterial()->SetTexture(0, new Texture2D(color));
			}
		}

		void OnDestroy()override {
			Debug::Log(L"##################################");
			Component::OnDestroy();
		}
	};
}