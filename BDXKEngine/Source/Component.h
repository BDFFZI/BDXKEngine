#pragma once
#include "Object.h"
#include "ComponentEvent.h"

class GameObject;
class Component :public Object
{
	friend GameObject;
public:

protected:
	virtual GameObject* GetGameObject() { return gameObject; }
private:
	GameObject* gameObject = 0;
};

