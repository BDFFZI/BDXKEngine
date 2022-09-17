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
			if (objectPtr.IsNull())
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
		bool operator ==(const ObjectPtrBase& other)const
		{
			Object* object = GetPtr();
			Object* otherObject = other.GetPtr();
			return object == otherObject;
		}
		bool operator !=(const ObjectPtrBase& other)const
		{
			return !(*this == other);
		}
		bool IsNull()const
		{
			return GetPtr() == nullptr;
		}

		//提取内容
		Object* GetPtr()const
		{
			return Object::FindObjectOfInstanceID(instanceID);
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
				Object* object = GetPtr();
				if (object != nullptr)
				{
					Object::Destroy(object);
				}
			}

			instanceID = 0;
		}
	};
}