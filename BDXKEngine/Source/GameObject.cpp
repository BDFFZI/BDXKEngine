#include "GameObject.h"

std::vector<GameObject*> GameObject::gameObjects;

 GameObject::GameObject(String name) {
	this->name = name;
	gameObjects.push_back(this);
}

void GameObject::OnUpdate()
{
	for (Component* component : startGameObjects)
	{
		component->OnStart();
		updateGameObjects.push_back(component);
	}
	startGameObjects.clear();

	for (Component* component : updateGameObjects)
		component->OnUpdate();

	for (Component* component : updateGameObjects)
		component->OnLateUpdate();

	for (Component* component : updateGameObjects)
		component->OnRenderObject();

	for (Component* component : updateGameObjects)
		component->OnDrawGizmos();
}
