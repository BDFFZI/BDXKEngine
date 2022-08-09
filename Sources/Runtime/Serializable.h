#pragma once
#include <string>
#include "ObjectPtr.h"

namespace BDXKEngine {
	template<typename TObject>
	class Serializable
	{
	public:
		ObjectPtr<TObject> Load(std::string string)
		{

		}

		std::string Save(ObjectPtr<TObject> object)
		{

		}
	};
}


