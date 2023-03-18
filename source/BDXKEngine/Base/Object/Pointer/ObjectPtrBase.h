#pragma once
#include <unordered_map>
#include "BDXKEngine/Base/Reflection/Reflection.h"
#include "BDXKEngine/Base/Reflection/Type.h"

namespace BDXKEngine
{
    typedef unsigned int ID;
    
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

        ID GetInstanceID() const;
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
        virtual void AddRef(ID refInstanceID);
        virtual void RemoveRef();
    private:
        inline static std::unordered_map<Type, std::uintptr_t> virtualTable = {};
        inline static std::unordered_map<ID, int> refCountMap = {};

        static void StaticConstructor()
        {
            const auto objectPtr = new ObjectPtrBase();
            SetVirtualTable(objectPtr->GetType(), objectPtr);
            delete objectPtr;
        }
        CustomStaticConstructor(StaticConstructor)

        ID instanceID = 0;
        Object* instancePtr = nullptr; //仅供调试用
    };
}
