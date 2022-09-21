#pragma once
#include "Base/Object/ObjectPtr.h"
#include "Platform/GL/Resources/TextureCube.h"

namespace BDXKEngine {
	struct GraphicsSettings {
		inline static ObjectPtr<TextureCube> skybox = nullptr;
	};
}