#include "Object.h"

namespace BDXKEngine {
	unsigned int Object::instanceIDCount = 0;
	std::map<unsigned int, Object*> Object::allObjects = {};
	std::map<unsigned int, Object*> Object::allObjectsRuntime = {};
	std::vector<Object*> Object::destroyBuffer;


	void Object::Awake(Object* object) {
		if (object == nullptr || object->isActivating)return;

		object->isActivating = true;
		allObjectsRuntime[object->instanceID] = object;
		object->Awake();
	}
	void Object::Destroy(Object* object) {
		if (object == nullptr || object->isDestroying)return;

		destroyBuffer.push_back(object);
		object->isDestroying = true;
		allObjectsRuntime.erase(object->instanceID);
		object->Destroy();
	}

	void Object::DestroyImmediate(Object* object) {
		std::vector<Object*> destroyBufferLast = destroyBuffer;

		destroyBuffer.clear();
		Destroy(object);
		FlushDestroyBuffer();

		destroyBuffer = destroyBufferLast;
	}
	Object* Object::FindObjectOfInstanceID(unsigned int instanceID)
	{
		auto pair = allObjects.find(instanceID);
		if (pair != allObjects.end())
			return pair->second;
		else
			return nullptr;
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

	bool Object::IsActivating()
	{
		return isActivating;
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
	}
	void Object::Destroy() {
		Debug::LogWarning((String)L"Object::Destroy " + instanceID + " " + GetName());
	}

	// 统一销毁被标记的物体，DestroyImmediate()调用，预留Unity延迟删除功能，应该是为了内存回收的问题
	void Object::FlushDestroyBuffer()
	{
		for (auto& object : destroyBuffer)
		{
			allObjects.erase(object->instanceID);
			delete object;
		}
		destroyBuffer.clear();
	}
}
