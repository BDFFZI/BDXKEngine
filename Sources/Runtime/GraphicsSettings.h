#pragma once
#include "ObjectPtr.h"
#include "TextureCube.h"

namespace BDXKEngine {
	struct GraphicsSettings {
		inline static ObjectPtr<TextureCube> skybox = nullptr;
	};
}