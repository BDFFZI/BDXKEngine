#include "ObjectPtrBase.h"
#include <iostream>
#include <sstream>
#include "Object.h"


namespace BDXKEngine
{
    std::map<int, int> ObjectPtrBase::refCountMap = {};

    void ObjectPtrBase::PrintRefCountMap()
    {
        std::stringstream stream = {};
        stream << "当前指针引用计数:" << std::to_string(refCountMap.size());
        for (const auto& value : refCountMap)
        {
            stream << "\n实例编号:" << std::to_string(value.first);
            stream << "\t引用计数:" << std::to_string(value.second);
        }
        std::cout << stream.str() << std::endl;
    }

    ObjectPtrBase::ObjectPtrBase()
    {
        instanceID = 0;
    }
    ObjectPtrBase::ObjectPtrBase(const Object* object)
    {
        if (object == nullptr)
        {
            instanceID = 0;
        }
        else
        {
            instanceID = object->GetInstanceID();
            if (refCountMap.count(instanceID) == 0)
                refCountMap[instanceID] = 0;

            ObjectPtrBase::AddRef(instanceID);
        }
    }
    ObjectPtrBase::ObjectPtrBase(const ObjectPtrBase& objectPtr)
    {
        if (objectPtr.instanceID != 0)
            ObjectPtrBase::AddRef(objectPtr.instanceID);
    }
    ObjectPtrBase::~ObjectPtrBase()
    {
        if (instanceID != 0)
            ObjectPtrBase::RemoveRef();
    }

    int ObjectPtrBase::GetInstanceID() const
    {
        return instanceID;
    }

    bool ObjectPtrBase::IsNull() const
    {
        return ToObjectBase() == nullptr;
    }
    bool ObjectPtrBase::IsNotNull() const
    {
        return ToObjectBase() != nullptr;
    }

    Object* ObjectPtrBase::ToObjectBase() const
    {
        return instanceID == 0 ? nullptr : Object::FindObjectOfInstanceID(instanceID);
    }

    Object* ObjectPtrBase::operator->() const
    {
        Object* object = ToObjectBase();
        if (object == nullptr)throw std::exception("当前物体指针的引用目标为空");
        return object;
    }
    bool ObjectPtrBase::operator==(const ObjectPtrBase& other) const
    {
        const Object* object = ToObjectBase();
        const Object* otherObject = other.ToObjectBase();
        return object == otherObject;
    }
    bool ObjectPtrBase::operator!=(const ObjectPtrBase& other) const
    {
        return !(*this == other);
    }
    ObjectPtrBase& ObjectPtrBase::operator=(const ObjectPtrBase& objectPtr)
    {
        if (this == &objectPtr)
            return *this;

        if (instanceID != 0)
            RemoveRef();

        if (objectPtr.instanceID != 0)
            AddRef(objectPtr.instanceID);

        return *this;
    }

    void ObjectPtrBase::AddRef(const int refInstanceID)
    {
        instanceID = refInstanceID;
        refCountMap[instanceID]++;
        //Debug::LogWarning(
        //	L"ObjectPtrBase::AddRef 实例编号:"
        //	+ std::to_string(instanceID)
        //	+ "\t新计数:"
        //	+ std::to_string(refCountMap[instanceID])
        //);
    }
    void ObjectPtrBase::RemoveRef()
    {
        const int refCount = --refCountMap[instanceID];
        //Debug::LogWarning(
        //	L"ObjectPtrBase::RemoveRef 实例编号:"
        //	+ std::to_string(instanceID)
        //	+ "\t新计数:"
        //	+ std::to_string(refCount)
        //);
        if (refCount == 0)
        {
            refCountMap.erase(instanceID);
            const Object* object = ToObjectBase();
            if (object != nullptr && object->IsDestroyed() == false)
            {
                Object::DestroyImmediate(object);
            }
        }

        instanceID = 0;
    }
}
