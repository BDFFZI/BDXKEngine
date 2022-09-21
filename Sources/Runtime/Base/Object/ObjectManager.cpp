#include "ObjectManager.h"
#include "Object.h"

void BDXKEngine::ObjectManager::FlushDestroyBuffer()
{
	Object::FlushDestroyBuffer();
}

std::wstring BDXKEngine::ObjectManager::InstanceIDToGuid(unsigned int instanceID) {
	return std::wstring();
}

unsigned int BDXKEngine::ObjectManager::GuidToInstanceID(std::wstring guid) {
	return 0;
}
