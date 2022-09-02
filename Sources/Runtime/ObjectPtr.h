#pragma once
#include <exception>
#include <unordered_map>
#include "Object.h"

namespace BDXKEngine {
	struct ObjectPtrBase {
	public:
		ObjectPtrBase() {
			instanceID = 0;
		}
		ObjectPtrBase(Object* object)
		{
			if (object == nullptr)
			{
				instanceID = 0;
			}
			else
			{
				instanceID = object->GetInstanceID();
				auto refCount = refCountMap.find(instanceID);
				if (refCount == refCountMap.end())
					refCountMap[instanceID] = 1;
				else
					refCountMap[instanceID] = refCount->second + 1;
			}
		}
		ObjectPtrBase(const ObjectPtrBase& objectPtr) {
			if ((ObjectPtrBase)objectPtr == nullptr)
			{
				instanceID = 0;
			}
			else
			{
				AddRef(objectPtr);
			}
		}
		ObjectPtrBase& operator=(const ObjectPtrBase& objectPtr)
		{
			if (instanceID != 0)
				RemoveRef();

			if (objectPtr.instanceID != 0)
				AddRef(objectPtr);

			return *this;
		}

		~ObjectPtrBase()
		{
			if (instanceID != 0)
				RemoveRef();
		}

		//内容比较
		bool operator ==(const ObjectPtrBase& other)
		{
			Object* object = *this;
			Object* otherObject = (ObjectPtrBase)other;
			return object == otherObject;
		}
		bool operator !=(const ObjectPtrBase& other)
		{
			return !(*this == other);
		}
		bool operator ==(const std::nullptr_t other) {
			return Object::FindObjectOfInstanceID(instanceID) == nullptr;
		}
		bool operator !=(const std::nullptr_t other) {
			return Object::FindObjectOfInstanceID(instanceID) != nullptr;
		}

		//提取内容
		operator Object* ()
		{
			return Object::FindObjectOfInstanceID(instanceID);
		}

		template<typename TTargetObject>
		ObjectPtr<TTargetObject> As()
		{
			Object* object = *this;
			return dynamic_cast<TTargetObject*>(object);
		}
	protected:
		unsigned int instanceID;
	private:
		inline static std::map<unsigned int, int> refCountMap;

		void AddRef(const ObjectPtrBase& objectPtr)
		{
			instanceID = objectPtr.instanceID;
			refCountMap[instanceID]++;
		}
		void RemoveRef()
		{
			int refCount = refCountMap[instanceID]--;
			if (refCount == 0)
			{
				refCountMap.erase(instanceID);
				Object* object = *this;
				if (object != nullptr)
				{
					Object::Destroy(object);
				}
			}

			instanceID = 0;
		}
	};

	template<typename TObject>
	struct ObjectPtr :public ObjectPtrBase
	{
	public:
		ObjectPtr() :ObjectPtrBase() {
		}
		ObjectPtr(Object* object) :ObjectPtrBase(object)
		{
		}
		ObjectPtr(const ObjectPtrBase& objectPtr) :ObjectPtrBase(objectPtr) {
		}


		TObject* operator->()const
		{
			Object* object = *this;
			if (object == nullptr)
				throw std::exception("目标引用为空");
			return static_cast<TObject*>(object);
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