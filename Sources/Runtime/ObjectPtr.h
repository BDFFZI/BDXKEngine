#pragma once
#include "ObjectPtrBase.h"

namespace BDXKEngine {
	template<typename TObject>
	struct ObjectPtr :public ObjectPtrBase
	{
	public:
		ObjectPtr() :ObjectPtrBase() {
		}
		ObjectPtr(Object* object) :ObjectPtrBase(object)
		{
		}
		ObjectPtr(const ObjectPtrBase& objectPtr) :ObjectPtrBase(objectPtr) {
		}

		template<typename TTargetObject>
		ObjectPtr<TTargetObject> As()
		{
			Object* object = GetPtr();
			return dynamic_cast<TTargetObject*>(object);
		}
		TObject* operator->()const
		{
			Object* object = GetPtr();
			if (object == nullptr)
				throw std::exception("目标引用为空");
			return static_cast<TObject*>(object);
		}
	};
}

//class Data :public Object {
//public:
//	Data(String name)
//	{
//		this->name = name;
//		Debug::Log(L"Create " + name);
//	}
//
//	~Data()
//	{
//		Debug::Log(L"Delete " + name);
//	}
//
//	String name;
//};
//
//int main()
//{
//	ObjectPtr<Data> A = { new Data{ L"A"} };
//	ObjectPtr<Object> A0 = { (Object*)A };
//	A0 = nullptr;
//	ObjectPtr<Data> B = A;
//	Debug::Log(A->name);
//	Object::DestroyImmediate(B);
//	if (A != nullptr)Debug::Log(A->name);
//
//	ObjectPtr<Data> C = { new Data{ L"C"} };
//	ObjectPtr<Data> D = C;
//	Debug::Log(C->name);
//	C = nullptr;
//	Debug::Log(C == B);
//	Debug::Log(C == D);
//}