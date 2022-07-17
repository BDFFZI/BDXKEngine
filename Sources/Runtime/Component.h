#pragma once
#include "ObjectPtr.h"

namespace BDXKEngine {
	class GameObject;
	class Transform;
	//确保TComponent有默认构造函数
	class Component :public Object
	{
		friend GameObject;
	public:
		//设为虚函数是为了转为多态类型
		ObjectPtr<GameObject> GetGameObject();
		ObjectPtr<Transform> GetTransform();
	protected:

	private:
		ObjectPtr<GameObject> gameObject = 0;
	};
}
