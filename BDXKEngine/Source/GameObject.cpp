#include "GameObject.h"

List<GameObject*> GameObject::gameObjects;
List<Component*> GameObject::components;
List<StartEvent*> GameObject::startEvents;
List<UpdateEvent*> GameObject::updateEvents;
List<LateUpdateEvent*> GameObject::lateUpdateEvents;
List<OnRenderObjectEvent*> GameObject::onRenderObjectEvents;
List<OnDrawGizmosEvent*> GameObject::onDrawGizmosEvents;

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
