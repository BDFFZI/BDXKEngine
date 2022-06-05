#include "GameObject.h"

std::vector<GameObject*> GameObject::gameObjects;
std::vector<Component*> GameObject::components;
std::vector<StartEvent*> GameObject::startEvents;
std::vector<UpdateEvent*> GameObject::updateEvents;
std::vector<LateUpdateEvent*> GameObject::lateUpdateEvents;
std::vector<OnRenderObjectEvent*> GameObject::onRenderObjectEvents;
std::vector<OnDrawGizmosEvent*> GameObject::onDrawGizmosEvents;

GameObject::GameObject(const wchar_t* name) {
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
	for (StartEvent* pevent : startEvents)
		pevent->Start();
	startEvents.clear();

	for (UpdateEvent* pevent : updateEvents)
		pevent->Update();

	for (LateUpdateEvent* pevent : lateUpdateEvents)
		pevent->LateUpdate();

	for (OnRenderObjectEvent* pevent : onRenderObjectEvents)
		pevent->OnRenderObject();

	for (OnDrawGizmosEvent* pevent : onDrawGizmosEvents)
		pevent->OnDrawGizmos();
}
