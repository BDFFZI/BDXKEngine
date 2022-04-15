#include "GameObject.h"

std::vector<GameObject*> GameObject::gameObjects;

 GameObject::GameObject(String name) {
	this->name = name;
	gameObjects.push_back(this);
}

void GameObject::Update()
{
	for (Component* component : startGameObjects)
	{
		component->Start();
		updateGameObjects.push_back(component);
	}
	startGameObjects.clear();

	for (Component* component : updateGameObjects)
		component->Update();

	for (Component* component : updateGameObjects)
		component->LateUpdate();

	for (Component* component : updateGameObjects)
		component->OnRenderObject();

	for (Component* component : updateGameObjects)
		component->OnDrawGizmos();
}
