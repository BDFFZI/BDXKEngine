#pragma once
#include <map>
#include <functional>
#include "ObjectPtr.h"

#define GetResourcesPath(Type,Name) "../Resources/"#Type"/"#Name""
#define GetResourcesPathW(Type,Name) L"../Resources/"#Type"/"#Name""

namespace BDXKEngine {
	class Resources
	{
	public:
		//template<typename TResult>
		//static ObjectPtr<TResult> Load(std::wstring path)
		//{
		//	if(
		//	return creator[typeid(TResult).hash_code()]
		//}
	private:
		static std::map<size_t, std::function<ObjectPtr<Object>(std::wstring path)>> creator;
	};

}

