#pragma once
#include "Component.h"
#include "String.h"
#include "Transform.h"
#include "List.h"
#include "Object.h"

class Component;
class BDXKEngine;
class GameObject :public Object
{
	friend BDXKEngine;
public:
	GameObject(String name = String(L""));

	Transform* GetTransform();

	template<typename TComponent>
	TComponent* AddComponent() {
		//确保TComponent实现相应的构造函数
		Component* component = new TComponent();
		component->gameObject = this;
		component->name = name + " : " + typeid(TComponent).name();
		component->Awake();
		components.push_back(component);
		startGameObjects.push_back(component);
		return (TComponent*)component;
	};
	template<typename TComponent>
	TComponent* GetComponent() {
		for (Component* component : components)
		{
			TComponent* target = dynamic_cast<TComponent*>(component);
			if (target != NULL)
				return target;
		}
		return NULL;
	}
private:
	static List<GameObject*> gameObjects;//所有物体

	Transform* transform;

	List<Component*> components;//所有组件
	List<Component*> startGameObjects;//待Start()的组件
	List<Component*> updateGameObjects;//进行Update()的组件

	void Update();
};

