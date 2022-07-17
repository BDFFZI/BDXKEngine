#pragma once
#include <exception>
#include <unordered_map>
#include "Object.h"
#include "Debug.h"

namespace BDXKEngine {
	inline static std::unordered_map<unsigned int, int> objectIDRefCount = {};

	template<typename TObject>
	struct ObjectPtr : ObjectEditor
	{
	public:
		ObjectPtr(TObject* target)
		{
			if (target == nullptr)
			{
				object = nullptr;
				objectID = 0;
			}
			else
			{
				object = target;
				objectID = ((Object*)target)->GetInstanceID();
				if (objectIDRefCount.count(objectID) == 0)
					objectIDRefCount[objectID] = 1;
				else
					objectIDRefCount[objectID]++;

				Debug::Log((String)L"+ " + objectID);
			}

		}
		ObjectPtr() :ObjectPtr(nullptr) {};
		ObjectPtr(const ObjectPtr& sharedPtr) :ObjectPtr(sharedPtr.object)
		{
		}
		~ObjectPtr()
		{
			if (object != nullptr)
				RemoveRef();
		}

		ObjectPtr& operator=(const ObjectPtr& sharedPtr)
		{
			if (object != nullptr)
				RemoveRef();

			if (sharedPtr.object != nullptr)
				AddRef(sharedPtr);

			return *this;
		}
		TObject* operator->()
		{
			if (IsNull())
				throw std::exception("目标引用为空");
			return object;
		}
		bool operator ==(ObjectPtr& other)
		{
			if (object == other.object)
				return true;
			else if (other.IsNull() && IsNull())
				return true;

			return false;
		}
		bool operator !=(ObjectPtr& other)
		{
			return !(*this == other);
		}

		template<typename TTargetObject>
		operator ObjectPtr<TTargetObject>()
		{
			return As<TTargetObject>();
		}
		operator TObject* () {
			return GetPtr();
		}

		template<typename TTargetObject>
		ObjectPtr<TTargetObject> As()
		{
			TObject* source = GetPtr();
			return dynamic_cast<TTargetObject*>(source);
		}
		TObject* GetPtr()
		{
			return IsNull() ? nullptr : object;
		}
	private:
		TObject* object;
		unsigned int objectID;

		void AddRef(const ObjectPtr& sharedPtr)
		{
			Debug::Log((String)L"+ " + objectID);

			object = sharedPtr.object;
			objectID = sharedPtr.objectID;
			objectIDRefCount[objectID]++;
		}
		void RemoveRef()
		{
			Debug::Log((String)L"- " + objectID);

			int refCount = objectIDRefCount[objectID] - 1;
			if (refCount == 0)
			{
				objectIDRefCount.erase(objectID);
				if (ObjectEditor::GetIDState(objectID))
					Object::Destroy((Object*)object);
			}
			else
			{
				objectIDRefCount[objectID] = refCount;
			}

			object = nullptr;
			objectID = 0;
		}
		bool IsNull()
		{
			return object == nullptr || ObjectEditor::GetIDState(objectID) == false;
		}
	};
}

//class Data :public Object {
//public:
//	Data(String name)
//	{
//		this->name = name;
//		Debug::Log(L"Create " + name);
//	}
//
//	~Data()
//	{
//		Debug::Log(L"Delete " + name);
//	}
//
//	String name;
//};
//
//int main()
//{
//	ObjectPtr<Data> A = { new Data{ L"A"} };
//	ObjectPtr<Object> A0 = { (Object*)A };
//	A0 = nullptr;
//	ObjectPtr<Data> B = A;
//	Debug::Log(A->name);
//	Object::Destroy(B);
//	if (A != nullptr)Debug::Log(A->name);
//
//	ObjectPtr<Data> C = { new Data{ L"C"} };
//	ObjectPtr<Data> D = C;
//	Debug::Log(C->name);
//	C = nullptr;
//	Debug::Log(C == B);
//	Debug::Log(C == D);
//}