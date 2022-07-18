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
			//初始化Component
			ObjectPtr<TComponent> component = { new TComponent() };
			component->gameObject = this;
			component->SetName((String)typeid(TComponent).name());
			//添加组件至自身
			components.push_back(component.As<Component>());
			//触发唤醒事件
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
		//所有物体(由GameObject负责增减)
		static std::vector<ObjectPtr<GameObject>> allGameObjects;

		//当前物体拥有的组件(由GameObject负责增减)
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