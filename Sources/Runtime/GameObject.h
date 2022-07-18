#pragma once
#include<vector>
#include<map>
#include<algorithm>
#include "String.h"
#include "ObjectPtr.h"
#include "Component.h"
#include "Transform.h"

namespace BDXKEngine {
	class Transform;
	class GameObjectEditor;
	class GameObject :public Object
	{
		friend GameObjectEditor;
	public:
		static ObjectPtr<GameObject> Find(std::wstring name);

		GameObject(std::wstring name = L"New GameObject");

		template<typename TComponent>
		ObjectPtr<TComponent> AddComponent() {
			//��ʼ��Component
			ObjectPtr<TComponent> component = { new TComponent() };
			component->gameObject = this;
			component->SetName((String)typeid(TComponent).name());
			//������������
			components.push_back(component.As<Component>());
			//���������¼�
			((Component*)component.GetPtr())->OnAwake();

			return component;
		};

		template<typename TComponent>
		ObjectPtr<TComponent> GetComponent() {
			for (ObjectPtr<Component>& component : components)
			{
				ObjectPtr<TComponent> target = component.As<TComponent>();
				if (target != nullptr)
					return target;
			}
			return nullptr;
		}
		std::vector<ObjectPtr<Component>> GetComponents();
		ObjectPtr<Transform> GetTransform();

	private:
		//��������(��GameObject��������)
		static std::vector<ObjectPtr<GameObject>> allGameObjects;

		//��ǰ����ӵ�е����(��GameObject��������)
		std::vector<ObjectPtr<Component>> components;

		void OnDestroy()override
		{
			for (ObjectPtr<Component>& component : components)
				Destroy(component);
			components.clear();
			allGameObjects.erase(std::find_if(
				allGameObjects.begin(),
				allGameObjects.end(),
				[=](ObjectPtr<GameObject>& item) {
					return item->GetInstanceID() == this->GetInstanceID();
				}
			));

			Object::OnDestroy();
		}
	};

	class GameObjectEditor {
	protected:
		static void Update();
		static void Release();
	};
}