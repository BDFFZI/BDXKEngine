#include "Object.h"
#include "ObjectPtr.h"
#include "Debug.h"

namespace BDXKEngine {
	unsigned int Object::instanceIDCount = 0;
	std::map<unsigned int, Object*> Object::allObjects = {};
	std::vector<Object*> Object::activateBuffer;
	std::vector<Object*> Object::destroyBuffer;

	Object::Object(std::wstring name) {
		this->name = name;
		instanceID = ++instanceIDCount;
		allObjects[instanceID] = this;

		Debug::Log((String)L"Object Create " + instanceID + " " + GetName());
	}
	Object::~Object() {
		Debug::Log((String)L"Object Delete " + instanceID + " " + GetName());
	}

	unsigned int Object::GetInstanceID() {
		return instanceID;
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
		return name + L"\n" + std::to_wstring(instanceID);
	}
}
