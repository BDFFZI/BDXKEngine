#pragma once
#include "Object.h"

class GameObject;
//ȷ��TComponent��Ĭ�Ϲ��캯��
class Component :public Object
{
	friend GameObject;
public:
	//��Ϊ�麯����Ϊ��תΪ��̬����
	virtual GameObject* GetGameObject() { return gameObject; }
protected:

private:
	GameObject* gameObject = 0;
};

