#pragma once
#include<vector>
#include<map>
#include<algorithm>
#include "String.h"
#include "ObjectPtr.h"
#include "Component.h"
#include "Transform.h"

namespace BDXKEngine {
	class WorldManager;
	class GameObject :public Object
	{
		friend Component;
		friend WorldManager;
	public:
		static ObjectPtr<GameObject> Create();

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

		template<typename TComponent>
		ObjectPtr<TComponent> AddComponent() {
			//��ʼ��Component
			TComponent source = {};
			source.gameObject = this;

			return Instantiate<TComponent>(&source);
		};

		static ObjectPtr<GameObject> Find(std::wstring name);
	private:
		//��������(��GameObject��������)
		static std::vector<ObjectPtr<GameObject>> allGameObjects;

		//��ǰ����ӵ�е����(��Component��������)
		std::vector<ObjectPtr<Component>> components;

		void Awake()override;
		void Destroy()override;
	};
}