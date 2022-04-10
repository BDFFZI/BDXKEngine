#pragma once
#include<vector>
#include "Component.h"
#include "String.h"
#include "Graphics.h"

class Component;
class BDXKEngine;
class GameObject
{
	friend BDXKEngine;
public:
	GameObject(String name = String(L""));;

	String name;

	template<typename TComponent>
	TComponent* AddComponent() {
		//ȷ��TComponentʵ����Ӧ�Ĺ��캯��
		Component* component = new TComponent();
		component->gameObject = this;
		component->OnAwake();
		components.push_back(component);
		startGameObjects.push_back(component);
		return (TComponent*)component;
	};
private:
	static std::vector<GameObject*> gameObjects;

	std::vector<Component*> components;
	std::vector<Component*> startGameObjects;
	std::vector<Component*> updateGameObjects;

	void OnUpdate();
};

