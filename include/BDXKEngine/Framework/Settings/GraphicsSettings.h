#pragma once
#include "BDXKEngine/Base/Object/ObjectPtr.h"
#include "BDXKEngine/Platform/GL/Resources/TextureCube.h"

namespace BDXKEngine {
	struct GraphicsSettings {
		inline static ObjectPtr<TextureCube> skybox = nullptr;
	};
}