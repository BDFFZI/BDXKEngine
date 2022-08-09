#pragma once
#include<algorithm>
#include "ObjectPtr.h"

namespace BDXKEngine {
	class WorldManager;
	class GameObject;
	class Transform;

	//ȷ��TComponent��Ĭ�Ϲ��캯��
	class Component :public Object
	{
		friend GameObject;
		friend WorldManager;
	public:
		Component(std::wstring name = L"New Component");

		//��Ϊ�麯����Ϊ��תΪ��̬����
		ObjectPtr<GameObject> GetGameObject();
		ObjectPtr<Transform> GetTransform();
	protected:
		//��ȫ����֮����дʱ����ػص�
		virtual void OnAwake();
		//��ȫɾ��֮ǰ����дʱ����ػص�
		void OnDestroy()override;
	private:
		ObjectPtr<GameObject> gameObject = nullptr;
	};
}
