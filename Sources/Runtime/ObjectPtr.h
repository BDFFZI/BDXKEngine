#pragma once
#include <exception>
#include <unordered_map>
#include "Object.h"

namespace BDXKEngine {
	template<typename TObject>
	struct ObjectPtr
	{
	public:
		ObjectPtr() {
			instanceID = 0;
			refCount = nullptr;
		}
		ObjectPtr(Object* object)
		{
			if (object == nullptr)
			{
				instanceID = 0;
				refCount = nullptr;
			}
			else
			{
				instanceID = object->GetInstanceID();
				refCount = new int(1);
			}
		}
		ObjectPtr(const ObjectPtr& objectPtr) {
			if ((Object*)objectPtr == nullptr)
			{
				instanceID = 0;
				refCount = nullptr;
			}
			else
			{
				AddRef(objectPtr);
			}
		}
		ObjectPtr& operator=(const ObjectPtr& objectPtr)
		{
			if (refCount != nullptr)
				RemoveRef();

			if (objectPtr.object != nullptr)
				AddRef(objectPtr);

			return *this;
		}

		~ObjectPtr()
		{
			if (refCount != nullptr)
				RemoveRef();
		}

		//内容比较
		bool operator ==(const ObjectPtr& other)
		{
			Object* object = *this;
			Object* otherObject = other;
			return object == otherObject;
		}
		bool operator !=(const ObjectPtr& other)
		{
			return !(*this == other);
		}

		//提取内容
		operator Object* ()
		{
			return Object::FindObjectOfInstanceID(instanceID);
		}
		TObject* operator->()const
		{
			Object* object = *this;
			if (object == nullptr)
				throw std::exception("目标引用为空");
			return static_cast<TObject*>(object);
		}
		template<typename TTargetObject>
		ObjectPtr<TTargetObject> As()
		{
			Object* object = *this;
			return dynamic_cast<TTargetObject*>(object);
		}
	private:
		unsigned int instanceID;
		unsigned int* refCount;

		void AddRef(const ObjectPtr& objectPtr)
		{
			instanceID = objectPtr.instanceID;
			refCount = objectPtr.refCount;
			*refCount++;
		}
		void RemoveRef()
		{
			*refCount--;
			if (*refCount == 0)
			{
				delete refCount;
				Object* object = (Object*)*this;
				if (object != nullptr)
				{
					Object::Destroy(object);
				}
			}

			instanceID = 0;
			refCount = nullptr;
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