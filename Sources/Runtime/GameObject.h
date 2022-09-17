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
	class WorldManager;
	class GameObject :public Object
	{
		friend Component;
		friend WorldManager;
	public:
		static ObjectPtr<GameObject> Find(std::wstring name);

		template<typename TComponent>
		ObjectPtr<TComponent> AddComponent() {
			//初始化Component
			ObjectPtr<TComponent> component = Instantiate<TComponent>(nullptr);
			component->gameObject = this;
			component->SetName((String)typeid(TComponent).name());

			return component;
		};

		template<typename TComponent>
		ObjectPtr<TComponent> GetComponent() {
			for (ObjectPtr<Component>& component : components)
			{
				ObjectPtr<TComponent> target = component.As<TComponent>();
				if (target.IsNull() == false)
					return target;
			}
			return nullptr;
		}
		ObjectPtr<Transform> GetTransform();

		std::vector<ObjectPtr<Component>> GetComponents();
	private:
		//所有物体(由GameObject负责增减)
		static std::vector<ObjectPtr<GameObject>> allGameObjects;

		//当前物体拥有的组件(由Component负责增减)
		std::vector<ObjectPtr<Component>> components;

		void Awake()override;
		void Destroy()override;
	};
}