#pragma once
#include "../Sources/Runtime/BDXKEngine.h"

namespace BDXKEditor {
	using namespace BDXKEngine;
	class SceneHUD :public Component, public PostRenderHandler, TransformEditor {
	private:
		std::wstring sceneInfo{ L"Hello BDXKEngine" };
		int frameRate = 0;

		float ShowGameObjectInfo(ObjectPtr<GameObject> target, float drawY, int order = 0)
		{
			float drawX = (float)(10 + order * 30);
			//显示GameObject
			{
				Rect rect = { drawX,drawY,160,25 };
				GUI::TextArea(rect, target->GetName());
				if (Event::IsFocus(rect))sceneInfo = target->ToString();
			}
			//添加Transform事件
			ObjectPtr<Transform> transform = target->GetTransform();
			{
				Rect transformRect = { drawX + 165,drawY,160,20 };
				//使其可拖拽
				if (Event::IsDrag(transformRect, transform.ToObjectPtr<Component>()))
				{
					GL2D::DrawRectangleCenter(Input::GetMousePosition(), transformRect.GetSize(), false);
				}
				//使其可拖入
				ObjectPtr<Transform> otherTransform = nullptr;
				if (Event::IsDrop(transformRect, (ObjectPtr<Component>*) & otherTransform))
				{
					if (otherTransform != nullptr)
					{
						try
						{
							otherTransform->SetParent(transform);
							Event::Use();
						}
						catch (std::exception e)
						{
							Debug::Log(e);
						}
					}
				}
			}
			//显示Components
			std::vector<ObjectPtr<Component>> components = target->GetComponents();
			for (ObjectPtr<Component> component : components)
			{
				drawX += 165;
				Rect rect = { drawX,drawY,160,20 };

				Component* componentPtr = component.ToObject();
				String fullName = typeid(*componentPtr).name();
				GUI::TextArea(rect, fullName.substr(fullName.find(L':') + 2), 15);

				if (Event::IsFocus(rect))sceneInfo = component->ToString();
			}
			//显示子物体
			if (transform->GetChildCount() != 0)
			{
				drawY += 30;
				for (int index = 0; index < transform->GetChildCount(); index++)
				{
					drawY = ShowGameObjectInfo(transform->GetChild(index)->GetGameObject(), drawY, order + 1);
				}
				return drawY;
			}

			return drawY + 30;
		}

		void OnPostRender()override
		{
			//显示所有游戏物体
			float drawY = 10;
			std::vector<ObjectPtr<Transform>> rootTransforms = TransformEditor::GetRootTransforms();
			for (ObjectPtr<Transform> transform : rootTransforms)
			{
				drawY = ShowGameObjectInfo(transform->GetGameObject(), drawY);
			}




			//显示描述
			Vector2 screenSize = Screen::GetSize();
			Rect rect{};
			rect.SetSize(screenSize / 4);
			rect.SetPosition({ screenSize.x - rect.width - 10, 10 });
			sceneInfo = GUI::TextArea(rect, sceneInfo);

			//显示帧率
			rect.y += rect.height + 10;
			rect.height = 25;
			GUI::TextArea(rect, L"帧率:" + std::to_wstring(frameRate = (int)std::lerp(frameRate, 1 / Time::GetDeltaTime(), 0.01f)));

			//孤儿箱:用来将节点父亲设为空
			rect.y += rect.height + 10;
			rect.height = 25;
			GUI::TextArea(rect, L"孤儿箱");
			ObjectPtr<Transform> transform = nullptr;
			if (Event::IsDrop(rect, (ObjectPtr<Component>*) & transform))
			{
				if (transform != nullptr)
				{
					transform->SetParent(nullptr);
				}
			}
		}
	};
}