#pragma once
#include <BDXKEngine.h>
#include "Editor/CreationMenu.h"
#include "AutoDestroy.h"

namespace Assembly {
	using namespace BDXKEngine;
	using namespace BDXKEditor;
	class CreateCube :public Component, public PostRenderHandler {
		ObjectPtr<Transform> transform;

		void Awake()override {
			Component::Awake();
			transform = GetTransform();
		}

		void OnPostRender()override {
			Vector2 size = Screen::GetSize();

			if (GUI::Button({ 10,size.y - 40,180,30 }, L"放置方块"))
			{
				ObjectPtr<GameObject> cube = CreationMenu::Object3D::Cube(L"自毁装置");
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

			GUI::TextArea({ 200,size.y - 40,200,30 }, L"鼠标位置:" + Input::GetMousePosition().ToString());
		}
	};
}