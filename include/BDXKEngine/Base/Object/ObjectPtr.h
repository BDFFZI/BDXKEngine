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
            debug = static_cast<TObject*>(ToObjectBase());
        }

        ObjectPtr(const ObjectPtr& other): ObjectPtrBase(other)
        {
            debug = other.debug;
        }

        ObjectPtr& operator=(const ObjectPtr& other)
        {
            if (this == &other)
                return *this;
            ObjectPtrBase::operator =(other);
            debug = other.debug;
            return *this;
        }

        template <typename TTargetObject>
        ObjectPtr<TTargetObject> ToObjectPtr() const
        {
            Object* object = ToObjectBase();
            return dynamic_cast<TTargetObject*>(object);
        }

        TObject* ToObject()
        {
            return static_cast<TObject*>(ToObjectBase());
        }

        TObject* operator->() const
        {
            Object* object = ToObjectBase();
            if (object == nullptr)
                throw std::exception("当前物体指针的引用目标为空");
            return static_cast<TObject*>(object);
        }

        ~ObjectPtr() override
        {
            debug = nullptr;
        }
    protected:
        TObject* debug = nullptr;

        void AddRef(const int refInstanceID) override
        {
            ObjectPtrBase::AddRef(refInstanceID);

            debug = reinterpret_cast<TObject*>(ToObjectBase());
        }

        void RemoveRef() override
        {
            ObjectPtrBase::RemoveRef();
            debug = nullptr;
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
