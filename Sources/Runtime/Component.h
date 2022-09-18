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
		//��Ϊ�麯����Ϊ��תΪ��̬����
		ObjectPtr<GameObject> GetGameObject();
		ObjectPtr<Transform> GetTransform();
	protected:
		void Export(Exporter& exporter) override;
		void Import(Importer& importer) override;
		//��ȫ����֮����дʱ����ػص�
		void Awake() override;
		//��ȫɾ��֮ǰ����дʱ����ػص�
		void Destroy()override;
	private:
		ObjectPtr<GameObject> gameObject = nullptr;
	};
}
