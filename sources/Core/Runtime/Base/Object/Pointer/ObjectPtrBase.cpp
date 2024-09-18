#include "ObjectPtrBase.h"
#include "Core/Runtime/Base/Object/Object.h"

namespace BDXKEngine
{
    ObjectPtrBase::ObjectPtrBase()
    {
        objectID = 0;
    }
    ObjectPtrBase::ObjectPtrBase(const Object* object)
    {
        if (object == nullptr)
        {
            objectID = 0;
        }
        else
        {
            objectID = object->GetObjectID();
            if (refCountMap.contains(objectID) == false)
                refCountMap[objectID] = 0;

            AddRef(objectID);
        }
    }
    ObjectPtrBase::ObjectPtrBase(const ObjectPtrBase& objectPtr)
    {
        if (objectPtr.objectID != 0)
            AddRef(objectPtr.objectID);
    }
    ObjectPtrBase& ObjectPtrBase::operator=(const ObjectPtrBase& objectPtr)
    {
        if (this == &objectPtr)
            return *this;

        if (objectID != 0)
            RemoveRef();

        if (objectPtr.objectID != 0)
            AddRef(objectPtr.objectID);

        return *this;
    }
    ObjectPtrBase::~ObjectPtrBase()
    {
        if (objectID != 0)
            RemoveRef();
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

    ID ObjectPtrBase::GetObjectID() const
    {
        return objectID;
    }
    Type ObjectPtrBase::GetObjectType() const
    {
        const Object* object = ToObjectBase();
        if (object == nullptr)return GetTypeOf<Object>();
        return object->GetType();
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
        return Object::FindObjectOfObjectID(objectID);
    }
    void ObjectPtrBase::AddRef(const ID refObjectID)
    {
        refCountMap[refObjectID]++;

        objectID = refObjectID;
        instancePtr = ToObjectBase();
    }
    void ObjectPtrBase::RemoveRef()
    {
        const int refCount = --refCountMap[objectID];
        if (refCount == 0)
        {
            refCountMap.erase(objectID);
            Object::DestroyObject(*this);
        }

        objectID = 0;
        instancePtr = nullptr;
    }
}
