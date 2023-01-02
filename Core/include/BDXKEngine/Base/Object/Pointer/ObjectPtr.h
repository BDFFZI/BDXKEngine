#pragma once
#include "ObjectPtrBase.h"
#include "BDXKEngine/Base/Reflection/Reflection.h"

namespace BDXKEngine
{
    template <typename TObject>
    class ObjectPtr : public ObjectPtrBase
    {
    public:
        ObjectPtr() : ObjectPtrBase()
        {
        }
        ObjectPtr(const TObject* object) : ObjectPtrBase(object)
        {
            debugPtr = static_cast<TObject*>(ToObjectBase());
        }
        ObjectPtr(const ObjectPtr& other): ObjectPtrBase(other)
        {
            debugPtr = other.debugPtr;
        }
        ~ObjectPtr() override
        {
            debugPtr = nullptr;
        }

        Type GetObjectType() const override
        {
            return GetTypeOf<TObject>();
        }

        template <typename TOtherObject>
        ObjectPtr<TOtherObject> ToObjectPtr() const
        {
            return ToObject<TOtherObject>();
        }

        TObject* operator->() const override
        {
            auto* object = ToObject<TObject>();
            if (object == nullptr)throw std::exception("当前物体指针的引用目标为空");
            return object;
        }
        bool operator ==(const ObjectPtr& other) const
        {
            const Object* object = ToObjectBase();
            const Object* otherObject = other.ToObjectBase();
            return object == otherObject;
        }
        bool operator !=(const ObjectPtr& other) const
        {
            return !(*this == other);
        }
        ObjectPtr& operator=(const ObjectPtr& other)
        {
            if (*this == other)
                return *this;

            ObjectPtrBase::operator =(other);
            debugPtr = other.debugPtr;
            return *this;
        }

    private:
        static void StaticConstructor()
        {
            ObjectPtr<TObject>* objectPtr = new ObjectPtr<TObject>();
            ObjectPtrBase::SetVirtualTable(objectPtr->GetType(), objectPtr);
            delete objectPtr;
        }
        CustomStaticConstructor(StaticConstructor)
        
        TObject* debugPtr = nullptr; //该值可能出错，仅供调试用

        void AddRef(const int refInstanceID) override
        {
            ObjectPtrBase::AddRef(refInstanceID);
            debugPtr = reinterpret_cast<TObject*>(ToObjectBase());
        }
        void RemoveRef() override
        {
            ObjectPtrBase::RemoveRef();
            debugPtr = nullptr;
        }
    };
}
