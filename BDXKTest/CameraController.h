#pragma once
#include "../Sources/Runtime/BDXKEngine.h"

namespace BDXKEditor {
	using namespace BDXKEngine;
	class CameraController :public Component, public StartEvent, public UpdateEvent, public DrawGizmosEvent {
		ObjectPtr<Transform> transform{};
		std::wstring text{ L"我是一个输入框。" };
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

		void OnDrawGizmos()override {
			Vector2 size = Screen::GetSize();
			if (GUI::Button({ 10,size.y - 40,180,30 }, L"按钮"))
			{
				Debug::Log(L"Hello BDXKEngine");
			}
			GUI::TextArea({ 200,size.y - 40,200,30 }, L"鼠标位置:" + Input::GetMousePosition().ToString());
			text = GUI::TextArea({ 410,size.y - 40,size.x - 420,30 }, text);

			GUI::TextArea({ size.x / 2 - 6,size.y / 2 - 6,3,3 }, L"");
		}
	};
}