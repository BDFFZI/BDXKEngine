#pragma once
#include "Object.h"
#include "ComponentEvent.h"

class GameObject;
class Component :public Object
{
	friend GameObject;
public:
	String ToString()override;
protected:
	virtual GameObject* GetGameObject() { return gameObject; }

	//virtual void Awake() {};
	//virtual void Start() {};
	//virtual void Update() {};
	//virtual void LateUpdate() {};
	//virtual void OnRenderObject() {};
	//virtual void OnDrawGizmos() {};
private:
	GameObject* gameObject = NULL;
};

