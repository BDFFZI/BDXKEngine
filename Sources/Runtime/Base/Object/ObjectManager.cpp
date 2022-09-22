﻿#include "ObjectManager.h"
#include "Object.h"

namespace BDXKEngine
{
	void ObjectManager::FlushDestroyBuffer()
	{
		Object::FlushDestroyQueue();
	}

	std::wstring ObjectManager::InstanceIDToGuid(unsigned int instanceID) {
		return {};
	}

	unsigned int ObjectManager::GuidToInstanceID(const std::wstring& guid) {
		return 0;
	}
}
