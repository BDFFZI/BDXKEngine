#pragma once
#include "Object.h"

namespace BDXKEngine {
	class GameObject;
	class Transform;
	//ȷ��TComponent��Ĭ�Ϲ��캯��
	class Component :public Object
	{
		friend GameObject;
	public:
		//��Ϊ�麯����Ϊ��תΪ��̬����
		GameObject* GetGameObject();
		Transform* GetTransform();
	protected:

	private:
		GameObject* gameObject = 0;
	};
}
