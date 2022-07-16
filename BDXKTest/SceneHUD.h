#pragma once
#include "../Sources/Runtime/BDXKEngine.h"

namespace BDXKEditor {
	using namespace BDXKEngine;
	class SceneHUD :public Component, public DrawGizmosEvent, TransformEditor {
	private:
		inline static std::wstring sceneInfo{ L"Hello BDXKEngine" };

		float ShowGameObjectInfo(GameObject* target, float drawY, int order = 0)
		{
			float drawX = (float)(10 + order * 30);
			//显示GameObject
			{
				Rect rect = { drawX,drawY,160,25 };
				GUI::TextArea(rect, target->GetName());
				if (Event::IsFocus(rect))sceneInfo = target->ToString();
			}
			//添加Transform事件
			Transform* transform = target->GetTransform();
			{
				Rect transformRect = { drawX + 165,drawY,160,20 };
				//使其可拖拽
				if (Event::IsDrag(transformRect, transform))
				{
					GL2D::DrawRectangleCenter(Input::GetMousePosition(), transformRect.GetSize(), false);
				}
				//使其可拖入
				Transform* otherTransform = nullptr;
				if (Event::IsDrop(transformRect, (Component**)&otherTransform))
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
			std::vector<Component*> components = target->GetComponents();
			for (Component* component : components)
			{
				drawX += 165;
				Rect rect = { drawX,drawY,160,20 };

				String fullName = typeid(*component).name();
				GUI::TextArea(rect, fullName.substr(fullName.find(L':') + 2), 15);

				if (Event::IsFocus(rect))sceneInfo = component->ToString();
			}
			//显示子物体
			if (transform->GetChildCount() != 0)
			{
				drawY += 25;
				for (int index = 0; index < transform->GetChildCount(); index++)
				{
					drawY = ShowGameObjectInfo(transform->GetChild(index)->GetGameObject(), drawY, order + 1);
				}
				return drawY;
			}

			return drawY + 30;
		}

		void OnDrawGizmos()override
		{
			float drawY = 10;
			std::vector<Transform*> rootTransforms = TransformEditor::GetRootTransforms();
			for (Transform* transform : rootTransforms)
			{
				drawY = ShowGameObjectInfo(transform->GetGameObject(), drawY);
			}

			//显示帧率
			GUI::TextArea({ 10,drawY,180,25 }, L"帧率:" + std::to_wstring(1 / Time::GetDeltaTime()));
			drawY += 30;

			//收集无父亲节点
			{
				Rect orphanBox{ 10,drawY,100,25 };
				GUI::TextArea(orphanBox, L"孤儿箱");
				Transform* transform = nullptr;
				if (Event::IsDrop(orphanBox, (Component**)&transform))
				{
					if (transform != nullptr)
					{
						transform->SetParent(nullptr);
					}
				}
			}


			//显示描述
			Vector2 screenSize = Screen::GetSize();
			Rect rect{};
			rect.SetSize(screenSize / 4);
			rect.SetPosition({ screenSize.x - rect.width - 10, 10 });
			sceneInfo = GUI::TextArea(rect, sceneInfo);
		}
	};
}