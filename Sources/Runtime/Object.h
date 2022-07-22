#pragma once
#include<vector>
#include<unordered_set>
#include<unordered_map>
#include<string>

namespace BDXKEngine {
	class ObjectEditor;
	class Object
	{
		friend ObjectEditor;
	public:
		template<typename TObject>
		static std::vector<TObject*> FindObjectsOfType()
		{
			std::vector<TObject*> result{};
			for (Object* object : objects) {
				TObject* item = dynamic_cast<TObject*>(object);
				if (item != NULL)result.push_back(item);
			}

			return result;
		}
		static void Destroy(Object* object);

		Object(std::wstring name = L"New Object");
		virtual ~Object();

		bool operator ==(Object* other)
		{
			if (instanceID == other->instanceID)
				return true;
			else if (IsNull() && other->IsNull())
				return true;

			return false;
		}
		bool operator !=(Object* other)
		{
			return !(*this == other);
		}

		unsigned int GetInstanceID();
		std::wstring GetName();

		void SetName(std::wstring name);

		virtual std::wstring ToString();
	protected:
		virtual void OnDestroy() {}
	private:
		static std::vector<Object*> objects;//��ǰ���ڵ�����
		static std::unordered_set<unsigned int> instanceIDStates;//����״�����Ƿ���ڣ��ж�
		static unsigned int instanceIDCount;

		unsigned int instanceID;
		std::wstring name;
		/// �Ƿ�ʹ�ù�Destroy������
		/// ����Destroyһ��������Դ������һ���̺ܿ��ܴ�������ָ���ɾ���������ظ�ɾ�������е�Object
		/// Ϊ��ֹ���������Destroy������ʹ�ø�ֵ���
		bool isDestroyed = false;

		bool IsNull();
	};

	class ObjectEditor {
	protected:
		bool GetIDState(unsigned int instanceID) const {
			return Object::instanceIDStates.count(instanceID) != 0;
		}
	};
}

