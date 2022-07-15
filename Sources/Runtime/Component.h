#pragma once
#include "Object.h"

namespace BDXKEngine {
	class GameObject;
	class Transform;
	//确保TComponent有默认构造函数
	class Component :public Object
	{
		friend GameObject;
	public:
		//设为虚函数是为了转为多态类型
		GameObject* GetGameObject();
		Transform* GetTransform();
	protected:

	private:
		GameObject* gameObject = 0;
	};
}
