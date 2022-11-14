#pragma once
#include "ObjectPtrBase.h"

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

        template <typename TOtherObject>
        ObjectPtr<TOtherObject> ToObjectPtr() const
        {
            return ToObject<TOtherObject>();
        }

        TObject* operator->() const
        {
            TObject* object = ToObject<TObject>();
            if (object == nullptr)throw std::exception("当前物体指针的引用目标为空");
            return object;
        }
        ObjectPtr& operator=(const ObjectPtr& other)
        {
            if (this == &other)
                return *this;

            ObjectPtrBase::operator =(other);
            debugPtr = other.debugPtr;
            return *this;
        }


    protected:
        TObject* debugPtr = nullptr;

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

//class Data :public Object {
//public:
//	Data(String name)
//	{
//		this->name = name;
//		Debug::Log("Create " + name);
//	}
//
//	~Data()
//	{
//		Debug::Log("Delete " + name);
//	}
//
//	String name;
//};
//
//int main()
//{
//	ObjectPtr<Data> A = { new Data{ "A"} };
//	ObjectPtr<Object> A0 = { (Object*)A };
//	A0 = nullptr;
//	ObjectPtr<Data> B = A;
//	Debug::Log(A->name);
//	Object::DestroyImmediate(B);
//	if (A != nullptr)Debug::Log(A->name);
//
//	ObjectPtr<Data> C = { new Data{ "C"} };
//	ObjectPtr<Data> D = C;
//	Debug::Log(C->name);
//	C = nullptr;
//	Debug::Log(C == B);
//	Debug::Log(C == D);
//}
