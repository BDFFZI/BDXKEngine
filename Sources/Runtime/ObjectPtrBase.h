#pragma once
#include <exception>
#include <unordered_map>
#include "Object.h"
#include "Debug.h"

namespace BDXKEngine {
	struct ObjectPtrBase {
	public:
		static void DebugRefCountMap()
		{
			std::wstringstream stream = {};
			for (auto value : refCountMap)
			{
				stream << L"实例编号:" << std::to_wstring(value.first);
				stream << L"\t";
				stream << L"引用计数:" << std::to_wstring(value.second);
				stream << L"\n";
			}
			Debug::LogWarning(stream.str());
		}

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
				if (refCountMap.find(instanceID) == refCountMap.end())
					refCountMap[instanceID] = 0;

				AddRef(instanceID);
			}
		}
		ObjectPtrBase(const ObjectPtrBase& objectPtr) {
			if (objectPtr.IsNull())
			{
				instanceID = 0;
			}
			else
			{
				AddRef(objectPtr.instanceID);
			}
		}
		ObjectPtrBase& operator=(const ObjectPtrBase& objectPtr)
		{
			if (instanceID != 0)
				RemoveRef();

			if (objectPtr.instanceID != 0)
				AddRef(objectPtr.instanceID);

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
			Object* object = GetObjectBase();
			Object* otherObject = other.GetObjectBase();
			return object == otherObject;
		}
		bool operator !=(const ObjectPtrBase& other)const
		{
			return !(*this == other);
		}
		bool IsNull()const
		{
			return GetObjectBase() == nullptr;
		}

		//提取内容
		int GetInstanceID()const {
			return instanceID;
		}
		Object* GetObjectBase()const
		{
			return Object::FindObjectOfInstanceID(instanceID);
		}
	protected:
		inline static std::map<unsigned int, int> refCountMap;

		unsigned int instanceID;

		virtual void AddRef(const unsigned int refInstanceID)
		{
			instanceID = refInstanceID;
			refCountMap[instanceID]++;
			//Debug::LogWarning(
			//	L"ObjectPtrBase::AddRef 实例编号:"
			//	+ std::to_wstring(instanceID)
			//	+ L"\t新计数:"
			//	+ std::to_wstring(refCountMap[instanceID])
			//);
		}
		virtual void RemoveRef()
		{
			int refCount = --refCountMap[instanceID];
			//Debug::LogWarning(
			//	L"ObjectPtrBase::RemoveRef 实例编号:"
			//	+ std::to_wstring(instanceID)
			//	+ L"\t新计数:"
			//	+ std::to_wstring(refCount)
			//);
			if (refCount == 0)
			{
				refCountMap.erase(instanceID);
				Object* object = GetObjectBase();
				if (object != nullptr)
				{
					Object::DestroyImmediate(object);
				}
			}

			instanceID = 0;
		}
	};
}