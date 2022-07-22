#include "Object.h"
#include "Debug.h"

namespace BDXKEngine {
	void Object::Destroy(Object* object)
	{
		if (object == nullptr || object->IsNull() || object->isDestroyed == true)
			return;

		object->isDestroyed = true;
		object->OnDestroy();

		instanceIDStates.erase(object->instanceID);//标记为不存在
		objects.erase(std::find(objects.begin(), objects.end(), object));//移除指针存档


		delete object;
	}


	Object::Object(std::wstring name) {
		this->name = name;
		instanceID = ++instanceIDCount;
		instanceIDStates.insert(instanceID);
		objects.push_back(this);

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

	inline bool Object::IsNull()
	{
		return instanceIDStates.count(instanceID) == 0;
	}

	std::vector<Object*> Object::objects = {};
	std::unordered_set<unsigned int> Object::instanceIDStates = {};
	unsigned int Object::instanceIDCount = 0;
}
