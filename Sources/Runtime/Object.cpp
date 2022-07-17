#include "Object.h"
#include "Debug.h"

namespace BDXKEngine {
	void Object::Destroy(Object* object)
	{
		if (object == nullptr || object->IsNull())
			return;

		DestoryEvent* destoryEvent = dynamic_cast<DestoryEvent*>(object);
		if (destoryEvent != nullptr)DestoryEventEditor::Destory(destoryEvent);

		instanceIDStates.erase(object->instanceID);
		objects.erase(std::find(objects.begin(), objects.end(), object));

		delete object;
	}


	Object::Object() {
		name = L"New Object";
		instanceID = ++instanceIDCount;
		instanceIDStates.insert(instanceID);
		objects.push_back(this);

		//Debug::Log((String)L"Create " + instanceID);
	}
	Object::~Object() {
		//Debug::Log((String)L"Delete " + instanceID);
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

	std::vector<Object*> Object::objects = {};
	std::unordered_set<unsigned int> Object::instanceIDStates = {};
	unsigned int Object::instanceIDCount = 0;
}
