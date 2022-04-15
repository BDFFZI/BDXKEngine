#pragma once
#include "GameObject.h"
#include "Graphics.h"

class GameObject;
class Component
{
	friend GameObject;
protected:
	virtual GameObject* GetGameObject() { return gameObject; }

	virtual void Awake() {};
	virtual void Start() {};
	virtual void Update() {};
	virtual void LateUpdate() {};
	virtual void OnRenderObject() {};
	virtual void OnDrawGizmos() {};
private:
	GameObject* gameObject = NULL;
};

