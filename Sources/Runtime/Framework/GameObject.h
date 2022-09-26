#pragma once
#include<vector>
#include "Base/Object/ObjectPtr.h"
#include "Framework/Component.h"
#include "Framework/Components/Transform.h"

namespace BDXKEngine {
	class GameObjectManager;
	class GameObject :public Object
	{
		friend Component;
		friend GameObjectManager;
	public:
		static ObjectPtr<GameObject> Create(std::wstring name = L"New GameObject");

		template<typename TComponent>
		ObjectPtr<TComponent> GetComponent() {
			for (ObjectPtr<Component>& component : components)
			{
				ObjectPtr<TComponent> target = component.ToObjectPtr<TComponent>();
				if (target.IsNull() == false)
					return target;
			}
			return nullptr;
		}
		std::vector<ObjectPtr<Component>> GetComponents();
		ObjectPtr<Transform> GetTransform();
		bool GetIsActivatingSelf() const;
		bool GetIsActivatingInHierarchy();

		void SetIsActivating(bool state);

		template<typename TComponent>
		ObjectPtr<TComponent> AddComponent() {
			//初始化Component
			TComponent source = {};
			source.gameObject = this;

			return Instantiate<TComponent>(&source);
		}

		static ObjectPtr<GameObject> Find(std::wstring name);
	private:
		//所有物体(由GameObject负责增减)
		static std::vector<ObjectPtr<GameObject>> allGameObjects;

		//当前物体拥有的组件(由Component负责增减)
		std::vector<ObjectPtr<Component>> components;
		bool isActivating = false;

		void Awake()override;
		void Destroy()override;
		void SetEnabling(bool state) override;
	};
}