#pragma once
#include "../Sources/Runtime/BDXKEngine.h"
#include "CreationMenu.h"
#include "AutoDestroy.h"

namespace Assembly {
	using namespace BDXKEngine;
	using namespace BDXKEditor;
	class CreateCube :public Component, public DrawGizmosEvent {
		ObjectPtr<Transform> transform;

		void OnAwake()override {
			Component::OnAwake();
			transform = GetTransform();
		}

		void OnDrawGizmos()override {
			Vector2 size = Screen::GetSize();

			if (GUI::Button({ 10,size.y - 40,180,30 }, L"放置方块"))
			{
				ObjectPtr<Transform> cube = CreationMenu::Object3D::Cube(L"自毁装置")->GetTransform();
				cube->SetLocalPosition(transform->GetPosition());
				cube->SetLocalEulerAngles(transform->GetEulerAngles());
				cube->GetGameObject()->AddComponent<AutoDestroy>();
			}

			GUI::TextArea({ 200,size.y - 40,200,30 }, L"鼠标位置:" + Input::GetMousePosition().ToString());
		}
	};
}