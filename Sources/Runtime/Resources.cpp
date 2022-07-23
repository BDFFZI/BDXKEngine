#include "Resources.h"
#include "Shader.h"
using namespace BDXKEngine;

std::map<size_t, std::function<ObjectPtr<Object>(std::wstring path)>> Resources::creator = {
	{
		typeid(Shader).hash_code(),
		[](std::wstring path) {
			return new Shader(
				path + L"/VertexShader.hlsl",
				path + L"/PixelShader.hlsl",
				PassType::ForwardBase
			);
		}
	},
	{

	}
};