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

			if (GUI::Button({ 10,size.y - 40,180,30 }, L"���÷���"))
			{
				ObjectPtr<GameObject> cube = CreationMenu::Object3D::Cube(L"�Ի�װ��");
				ObjectPtr<Transform> cubeTransform = cube->GetTransform();
				cubeTransform->SetLocalPosition(transform->GetPosition());
				cubeTransform->SetLocalEulerAngles(transform->GetEulerAngles());
				cubeTransform->GetGameObject()->AddComponent<AutoDestroy>();
				ObjectPtr<Renderer> meshRender = cube->GetComponent<Renderer>();
				ObjectPtr<Material> material = meshRender->GetMaterial();
				material->SetRenderQueue(RenderQueue::Transparent);
				std::vector<ObjectPtr<Shader>> shaders = material->GetShaders();
				shaders[0]->SetBlend(Blend::Multiply);
			}

			GUI::TextArea({ 200,size.y - 40,200,30 }, L"���λ��:" + Input::GetMousePosition().ToString());
		}
	};
}