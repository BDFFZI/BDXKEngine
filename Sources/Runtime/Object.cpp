#include "Object.h"

namespace BDXKEngine {
	std::vector<Object*> Object::objects = {};

	int Object::GetID() {
		return id;
	}
	std::wstring Object::GetName()
	{
		return name;
	}

	void Object::SetName(std::wstring name)
	{
		this->name = name;
	}
	
	std::wstring Object::ToString()
	{
		return name + L"\n" + std::to_wstring(id);
	}

	//void ObjectEditor::InitializeObject(Object* object, int id, std::wstring name)
	//{
	//	object->id = id;
	//	object->name = name;
	//}
	//void ObjectEditor::SetID(Object* object, int id)
	//{
	//	object->id = id;
	//}
}
