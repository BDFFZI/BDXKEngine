#pragma once
#include<algorithm>
#include "ObjectPtr.h"

namespace BDXKEngine {
	class WorldManager;
	class GameObject;
	class Transform;

	//确保TComponent有默认构造函数
	class Component :public Object
	{
		friend GameObject;
		friend WorldManager;
	public:
		Component(std::wstring name = L"New Component");

		//设为虚函数是为了转为多态类型
		ObjectPtr<GameObject> GetGameObject();
		ObjectPtr<Transform> GetTransform();
	protected:
		//完全创建之后。重写时请务必回调
		virtual void OnAwake();
		//完全删除之前。重写时请务必回调
		void OnDestroy()override;
	private:
		ObjectPtr<GameObject> gameObject = nullptr;
	};
}
