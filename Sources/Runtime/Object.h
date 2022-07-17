#pragma once
#include<vector>
#include<unordered_set>
#include<string>
#include "DestroyEvent.h"

namespace BDXKEngine {
	class ObjectEditor;
	class Object :DestoryEventEditor
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

		Object();
		virtual ~Object();;

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
	private:
		static std::vector<Object*> objects;//��ǰ���ڵ�����
		static std::unordered_set<unsigned int> instanceIDStates;
		static unsigned int instanceIDCount;

		unsigned int instanceID;
		std::wstring name;

		bool IsNull()
		{
			return instanceIDStates.count(instanceID) == 0;
		}
	};

	class ObjectEditor {
	protected:
		bool GetIDState(unsigned int instanceID) {
			return Object::instanceIDStates.count(instanceID) != 0;
		}
	};
}

