#include "Object.h"

namespace BDXKEngine {
	unsigned int Object::instanceIDCount = 0;
	std::map<unsigned int, Object*> Object::allObjects = {};
	std::map<unsigned int, Object*> Object::allObjectsEnabling = {};
	std::vector<Object*> Object::activateBuffer;
	std::vector<Object*> Object::destroyBuffer;

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

	bool Object::IsInstantiated()
	{
		return isActivating && activateBuffer.size() == 0;
	}
	std::wstring Object::ToString()
	{
		return name + L"\n" + std::to_wstring(instanceID);
	}

	void Object::Export(Exporter& transfer) {
		transfer.TransferInt(instanceID);
		transfer.TransferString(name);
	}
	void Object::Import(Importer& transfer) {
		if (transfer.TransferInt() == 0)
			transfer.TransferString();
		else
			name = transfer.TransferString();
	}
	void Object::Awake() {
		Debug::LogWarning((String)L"Object::Awake " + instanceID + " " + GetName());

		isEnabling = true;
		allObjectsEnabling[instanceID] = this;
	}
	void Object::Destroy() {
		Debug::LogWarning((String)L"Object::Destroy " + instanceID + " " + GetName());

		allObjectsEnabling.erase(instanceID);
		isEnabling = false;
	}
}
