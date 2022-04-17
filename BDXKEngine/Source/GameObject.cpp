#include "GameObject.h"

List<GameObject*> GameObject::gameObjects;

GameObject::GameObject(String name) {
	this->name = name;
	transform = AddComponent<Transform>();
	gameObjects.push_back(this);
}

Transform* GameObject::GetTransform()
{
	return transform;
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
