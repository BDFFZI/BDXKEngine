#pragma once
#include "../Sources/Runtime/BDXKEngine.h"

namespace BDXKEditor {
	using namespace BDXKEngine;
	class SceneHUD :public Component, public DrawGizmosEvent, TransformEditor {
	private:
		inline static std::wstring sceneInfo{ L"Hello BDXKEngine" };

		float ShowGameObjectInfo(ObjectPtr<GameObject> target, float drawY, int order = 0)
		{
			float drawX = (float)(10 + order * 30);
			//��ʾGameObject
			{
				Rect rect = { drawX,drawY,160,25 };
				GUI::TextArea(rect, target->GetName());
				if (Event::IsFocus(rect))sceneInfo = target->ToString();
			}
			//���Transform�¼�
			ObjectPtr<Transform> transform = target->GetTransform();
			{
				Rect transformRect = { drawX + 165,drawY,160,20 };
				//ʹ�����ק
				if (Event::IsDrag(transformRect, transform))
				{
					GL2D::DrawRectangleCenter(Input::GetMousePosition(), transformRect.GetSize(), false);
				}
				//ʹ�������
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
			//��ʾComponents
			std::vector<ObjectPtr<Component>> components = target->GetComponents();
			for (ObjectPtr<Component> component : components)
			{
				drawX += 165;
				Rect rect = { drawX,drawY,160,20 };

				String fullName = typeid(*component.GetPtr()).name();
				GUI::TextArea(rect, fullName.substr(fullName.find(L':') + 2), 15);

				if (Event::IsFocus(rect))sceneInfo = component->ToString();
			}
			//��ʾ������
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

		void OnDrawGizmos()override
		{
			float drawY = 10;
			std::vector<ObjectPtr<Transform>> rootTransforms = TransformEditor::GetRootTransforms();
			for (ObjectPtr<Transform> transform : rootTransforms)
			{
				drawY = ShowGameObjectInfo(transform->GetGameObject(), drawY);
			}

			//��ʾ֡��
			GUI::TextArea({ 10,drawY,180,25 }, L"֡��:" + std::to_wstring(1 / Time::GetDeltaTime()));
			drawY += 30;

			//�¶���:�������ڵ㸸����Ϊ��
			{
				Rect orphanBox{ 10,drawY,100,25 };
				GUI::TextArea(orphanBox, L"�¶���");
				ObjectPtr<Transform> transform = nullptr;
				if (Event::IsDrop(orphanBox, (ObjectPtr<Component>*) & transform))
				{
					if (transform != nullptr)
					{
						transform->SetParent(nullptr);
					}
				}
			}


			//��ʾ����
			Vector2 screenSize = Screen::GetSize();
			Rect rect{};
			rect.SetSize(screenSize / 4);
			rect.SetPosition({ screenSize.x - rect.width - 10, 10 });
			sceneInfo = GUI::TextArea(rect, sceneInfo);
		}
	};
}