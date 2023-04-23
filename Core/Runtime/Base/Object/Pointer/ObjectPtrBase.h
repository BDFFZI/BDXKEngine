#pragma once
#include <unordered_map>
#include "Core/Runtime/Base/Reflection/Type.h"

namespace BDXKEngine
{
    typedef unsigned int ID;

    class Object;

    class ObjectPtrBase
    {
    public:
        ObjectPtrBase();
        ObjectPtrBase(const Object* object);
        ObjectPtrBase(const ObjectPtrBase& objectPtr);
        ObjectPtrBase& operator=(const ObjectPtrBase& objectPtr);
        ~ObjectPtrBase();

        Object* operator->() const;
        bool operator ==(const ObjectPtrBase& other) const;
        bool operator !=(const ObjectPtrBase& other) const;

        ID GetObjectID() const;
        Type GetObjectType() const; //TODO 作为空指针时无法获取子类类型

        bool IsNull() const;
        bool IsNotNull() const;

        template <typename TObject=Object>
        TObject* ToObject() const
        {
            return dynamic_cast<TObject*>(ToObjectBase());
        }

    protected:
        Object* ToObjectBase() const;
        void AddRef(ID refObjectID);
        void RemoveRef();

    private:
        inline static std::unordered_map<ID, int> refCountMap = {};

        ID objectID = 0;
        Object* instancePtr = nullptr; //仅供调试用
    };
}
