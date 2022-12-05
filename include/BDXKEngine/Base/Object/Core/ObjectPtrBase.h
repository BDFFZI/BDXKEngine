#pragma once
#include <unordered_map>
#include "BDXKEngine/Base/Reflection/Reflection.h"
#include "BDXKEngine/Base/Reflection/Type.h"

namespace BDXKEngine
{
    class Object;

    class ObjectPtrBase
    {
    public:
        static void SetVirtualTable(const Type& type, ObjectPtrBase* objectPtrBase);
        static void GetVirtualTable(const Type& type, ObjectPtrBase* objectPtrBase);
        static void PrintRefCountMap();

        ObjectPtrBase();
        ObjectPtrBase(const Object* object);
        ObjectPtrBase(const ObjectPtrBase& objectPtr);
        virtual ~ObjectPtrBase();

        int GetInstanceID() const;
        Type GetType() const;
        virtual Type GetObjectType() const;

        bool IsNull() const;
        bool IsNotNull() const;

        Object* ToObjectBase() const;
        template <typename TObject>
        TObject* ToObject() const
        {
            return dynamic_cast<TObject*>(ToObjectBase());
        }

        virtual Object* operator->() const;
        bool operator ==(const ObjectPtrBase& other) const;
        bool operator !=(const ObjectPtrBase& other) const;
        ObjectPtrBase& operator=(const ObjectPtrBase& objectPtr);
    protected:
        virtual void AddRef(int refInstanceID);
        virtual void RemoveRef();
    private:
        inline static std::unordered_map<Type, std::uintptr_t> virtualTable = {};
        inline static std::unordered_map<int, int> refCountMap = {};

        static void StaticConstructor();
        CustomStaticConstructor(StaticConstructor)

        int instanceID = 0;
    };
}
