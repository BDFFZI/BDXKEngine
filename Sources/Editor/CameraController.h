#pragma once
#include <BDXKEngine.h>

namespace Assembly {
	using namespace BDXKEngine;
	class CameraController :public Behavior, public StartHandler, public UpdateHandler {
		ObjectPtr<Transform> transform{};

		void OnStart()override
		{
			transform = GetGameObject()->GetTransform();
		}

		void OnUpdate()override
		{
			Vector3 position = transform->GetLocalPosition();
			float deltaTime = Time::GetDeltaTime();

			if (Input::GetKey(KeyCode::W))
			{
				position += transform->GetFront().GetNormalized() * deltaTime * 4;
			}
			if (Input::GetKey(KeyCode::S))
			{
				position += -transform->GetFront().GetNormalized() * deltaTime * 4;
			}
			if (Input::GetKey(KeyCode::A))
			{
				position += -transform->GetRight().GetNormalized() * deltaTime * 4;
			}
			if (Input::GetKey(KeyCode::D))
			{
				position += transform->GetRight().GetNormalized() * deltaTime * 4;
			}
			if (Input::GetKey(KeyCode::Q))
			{
				position += -transform->GetUp().GetNormalized() * deltaTime * 4;
			}
			if (Input::GetKey(KeyCode::E))
			{
				position += transform->GetUp().GetNormalized() * deltaTime * 4;
			}

			transform->SetLocalPosition(position);

			if (Input::GetMouseButton(1))
			{
				Vector3 localEulerAngles = transform->GetLocalEulerAngles();
				Vector2 mouseMoveDelta = Input::GetMouseMoveDelta();

				localEulerAngles.y += mouseMoveDelta.x * deltaTime * 5;
				localEulerAngles.x += mouseMoveDelta.y * deltaTime * 5;

				if (Input::GetKey(KeyCode::Z))
				{
					localEulerAngles.z += deltaTime * 20;
				}
				if (Input::GetKey(KeyCode::X))
				{
					localEulerAngles.z -= deltaTime * 20;
				}

				transform->SetLocalEulerAngles(localEulerAngles);
			}

			if (Input::GetMouseButtonDown(1))
			{
				Cursor::SetLockState(true);
				Cursor::SetVisible(false);
			}
			if (Input::GetMouseButtonUp(1))
			{
				Cursor::SetLockState(false);
				Cursor::SetVisible(true);
			}
		}
	};
}