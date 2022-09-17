#pragma once
#include <string>

namespace BDXKEngine {
	class ObjectManager {
	protected:
		void FlushDestroyBuffer();
		static std::wstring InstanceIDToGuid(unsigned int instanceID);;
		static unsigned int GuidToInstanceID(std::wstring guid);;
	};
}
