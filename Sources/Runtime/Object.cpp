#include "Object.h"
#include "Debug.h"

namespace BDXKEngine {
	unsigned int Object::instanceIDCount = 0;
	std::map<unsigned int, Object*> Object::allObjects = {};
	std::vector<Object*> Object::activateBuffer;
	std::vector<Object*> Object::destroyBuffer;

	Object::Object() {
		instanceID = ++instanceIDCount;
		this->name = std::to_wstring(instanceID);
		allObjects[instanceID] = this;
	}
	Object::~Object() {
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

	void Object::Awake() {
		SetName((String)(typeid(*this).name()));
		Debug::Log((String)L"Object Awake " + instanceID + " " + GetName());
	}
	void Object::Destroy() {
		Debug::Log((String)L"Object Destroy " + instanceID + " " + GetName());
	}

	std::wstring Object::ToString()
	{
		return name + L"\n" + std::to_wstring(instanceID);
	}
}
