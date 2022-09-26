#pragma once
#include<algorithm>
#include "Base/Object/ObjectPtr.h"

namespace BDXKEngine {
	class GameObjectManager;
	class GameObject;
	class Transform;

	//确保TComponent有默认构造函数
	class Component :public Object
	{
		friend GameObject;
		friend GameObjectManager;
	public:
		//设为虚函数是为了转为多态类型
		ObjectPtr<GameObject> GetGameObject();
		ObjectPtr<Transform> GetTransform();
	protected:
		void Export(Exporter& exporter) override;
		void Import(Importer& importer) override;
		//完全创建之后。重写时请务必回调
		void Awake() override;
		//完全删除之前。重写时请务必回调
		void Destroy()override;
		void SetEnabling(bool state) override;
	private:
		ObjectPtr<GameObject> gameObject = nullptr;
	};
}
