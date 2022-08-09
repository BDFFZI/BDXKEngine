#pragma once
#include <exception>
#include <unordered_map>
#include "Object.h"

namespace BDXKEngine {
	//内联变量：确保全局唯一
	inline std::unordered_map<unsigned int, int> objectIDRefCount;

	/// 使用备注：
	/// 1.在构造函数中使用时记得SuppressDestroy
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
			}

		}
		ObjectPtr() :ObjectPtr(nullptr) {};
		ObjectPtr(const ObjectPtr& sharedPtr) :ObjectPtr(sharedPtr.object) {}
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
		TObject* operator->()const
		{
			if (IsNull())
				throw std::exception("目标引用为空");
			return object;
		}
		bool operator ==(const ObjectPtr& other)
		{
			if (object == other.object)
				return true;
			else if (other.IsNull() && IsNull())
				return true;

			return false;
		}
		bool operator !=(const ObjectPtr& other)
		{
			return !(*this == other);
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
		void SuppressDestroy()
		{
			isSuppressDestroy = true;
		}
	private:
		TObject* object;
		unsigned int objectID;
		bool isSuppressDestroy = false;

		void AddRef(const ObjectPtr& sharedPtr)
		{
			object = sharedPtr.object;
			objectID = sharedPtr.objectID;
			objectIDRefCount[objectID]++;
		}
		void RemoveRef()
		{
			int refCount = objectIDRefCount[objectID] - 1;
			if (refCount == 0)
			{
				objectIDRefCount.erase(objectID);
				if (ObjectEditor::GetIDState(objectID))
				{
					if (isSuppressDestroy == false)
						Object::DestroyImmediate((Object*)object);
					else
						isSuppressDestroy = false;
				}
			}
			else
			{
				objectIDRefCount[objectID] = refCount;
			}

			object = nullptr;
			objectID = 0;
		}
		bool IsNull()const
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
//	Object::DestroyImmediate(B);
//	if (A != nullptr)Debug::Log(A->name);
//
//	ObjectPtr<Data> C = { new Data{ L"C"} };
//	ObjectPtr<Data> D = C;
//	Debug::Log(C->name);
//	C = nullptr;
//	Debug::Log(C == B);
//	Debug::Log(C == D);
//}