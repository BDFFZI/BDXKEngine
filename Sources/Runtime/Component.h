#pragma once
#include "ObjectPtr.h"

namespace BDXKEngine {
	class GameObject;
	class Transform;
	//ȷ��TComponent��Ĭ�Ϲ��캯��
	class Component :public Object
	{
		friend GameObject;
	public:
		//��Ϊ�麯����Ϊ��תΪ��̬����
		ObjectPtr<GameObject> GetGameObject();
		ObjectPtr<Transform> GetTransform();
	protected:

	private:
		ObjectPtr<GameObject> gameObject = 0;
	};
}
