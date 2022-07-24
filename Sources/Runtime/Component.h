#pragma once
#include<algorithm>
#include "ObjectPtr.h"

namespace BDXKEngine {
	class GameObjectManager;
	class GameObject;
	class Transform;

	class StartEvent {
		friend GameObjectManager;
	protected:
		virtual void OnStart() = 0;
	};
	class UpdateEvent {
		friend GameObjectManager;
	protected:
		virtual void OnUpdate() = 0;
	};
	class LateUpdateEvent {
		friend GameObjectManager;
	protected:
		virtual void OnLateUpdate() = 0;
	};

	//确保TComponent有默认构造函数
	class Component :public Object
	{
		friend GameObject;
		friend GameObjectManager;
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
		//所有更新事件
		static std::vector<StartEvent*> allStartEvents;//由Component和GameObjectEditor控制
		static std::vector<UpdateEvent*> allUpdateEvents;//由Component控制
		static std::vector<LateUpdateEvent*> allLateUpdateEvents;//由Component控制

		ObjectPtr<GameObject> gameObject = nullptr;
	};
}
