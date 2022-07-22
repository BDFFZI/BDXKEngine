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
		friend Component;
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
			//触发唤醒事件
			((Component*)component.GetPtr())->OnAwake();

			return component;
		};


		std::vector<ObjectPtr<Component>> GetComponents();

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
		ObjectPtr<Transform> GetTransform();

	private:
		//所有物体(由GameObject负责增减)
		static std::vector<ObjectPtr<GameObject>> allGameObjects;

		//当前物体拥有的组件(由Component负责增减)
		std::vector<ObjectPtr<Component>> components;

		void OnDestroy()override;
	};

	class GameObjectEditor {
	protected:
		static void Update();
		static void Release();
	};
}