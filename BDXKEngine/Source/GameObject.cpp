#include "GameObject.h"

std::vector<GameObject*> GameObject::gameObjects;
std::vector<Component*> GameObject::components;
std::vector<StartEvent*> GameObject::startEvents;
std::vector<UpdateEvent*> GameObject::updateEvents;
std::vector<LateUpdateEvent*> GameObject::lateUpdateEvents;
std::vector<RenderObjectEvent*> GameObject::renderObjectEvents;
std::vector<DrawGizmosEvent*> GameObject::drawGizmosEvents;

GameObject::GameObject(const wchar_t* name) {
	ObjectEditor::InitializeObject(this, (int)gameObjects.size() * 100, name);
	gameObjects.push_back(this);
	AddComponent<Transform>();
}

Transform* GameObject::GetTransform()
{
	return GetComponent<Transform>();
}

void GameObjectEditor::OnUpdate()
{
	for (StartEvent* eventP : GameObject::startEvents)
		TickEventEditor::Start(eventP);
	GameObject::startEvents.clear();

	for (UpdateEvent* eventP : GameObject::updateEvents)
		TickEventEditor::Update(eventP);

	for (LateUpdateEvent* eventP : GameObject::lateUpdateEvents)
		TickEventEditor::LateUpdate(eventP);

	for (RenderObjectEvent* eventP : GameObject::renderObjectEvents)
		TickEventEditor::RenderObject(eventP);

	for (DrawGizmosEvent* eventP : GameObject::drawGizmosEvents)
		TickEventEditor::DrawGizmos(eventP);
}
