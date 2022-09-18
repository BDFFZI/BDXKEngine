#pragma once
#include "../Sources/Runtime/BDXKEngine.h"

namespace Assembly {
	using namespace BDXKEngine;
	class AutoDestroy :public Component, public UpdateHandler {
		float time;
		ObjectPtr<Animator> animator;
		ObjectPtr<MeshRenderer> meshRenderer;

		void Awake()override {
			Component::Awake();

			time = Time::GetRealtimeSinceStartup();
			meshRenderer = GetGameObject()->GetComponent<MeshRenderer>();
			animator = GetGameObject()->AddComponent<Animator>();

			animator->SetAnimation([](ObjectPtr<Transform> transform) {
				transform->SetLocalPosition(transform->GetLocalPosition() + transform->GetFront() * Time::GetDeltaTime() * 3);
				});
		}

		void OnUpdate()override {
			if (Time::GetRealtimeSinceStartup() - time > 2)
				DestroyImmediate(animator.ToObject());
			if (Time::GetRealtimeSinceStartup() - time > 4)
			{
				ObjectPtr<GameObject> gameObject = GetGameObject();
				DestroyImmediate(this);
				DestroyImmediate(gameObject.ToObject());
				return;
			}
			if ((int)std::fmodf(Time::GetFrameCount() - time, 100) == 0)
			{
				Debug::Log(std::to_wstring(GetInstanceID()) + L" Ëæ»ú:" + Random::ColorHSV().ToString());
				Color color = Random::ColorHSV();
				color.a = 0.3f;

				meshRenderer->GetMaterial()->SetTexture(0, Texture2D::Create(color).ToObjectPtr<Texture>());
			}
		}

		void Destroy()override {
			Debug::Log(L"##################################");
			Component::Destroy();
		}
	};
}