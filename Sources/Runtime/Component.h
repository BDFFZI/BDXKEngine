#pragma once
#include "Object.h"

class GameObject;
//确保TComponent有默认构造函数
class Component :public Object
{
	friend GameObject;
public:
	//设为虚函数是为了转为多态类型
	virtual GameObject* GetGameObject() { return gameObject; }
protected:

private:
	GameObject* gameObject = 0;
};

