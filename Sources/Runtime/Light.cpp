#include "Light.h"
using namespace BDXKEngine;

std::vector<ObjectPtr<Light>> Light::lights;

BDXKEngine::Light::Light():Component(L"New Light")
{
	lights.push_back(this);
	shadow = new Texture2D{ 1024,1024 };
}
