#pragma once
class GameObject;

class AwakeEvent {
	friend GameObject;
protected:
	virtual void Awake() = 0;
};

class StartEvent {
	friend GameObject;
protected:
	virtual void Start() = 0;
};

class UpdateEvent {
	friend GameObject;
protected:
	virtual void Update() = 0;
};

class LateUpdateEvent {
	friend GameObject;
protected:
	virtual void LateUpdate() = 0;
};

class OnRenderObjectEvent {
	friend GameObject;
protected:
	virtual void OnRenderObject() = 0;
};

class OnDrawGizmosEvent {
	friend GameObject;
protected:
	virtual void OnDrawGizmos() = 0;
};
