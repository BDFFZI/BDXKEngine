#pragma once
#include "ObjectPtr.h"
#include "TextureCube.h"

namespace BDXKEngine {
	struct GraphicsSettings {
		static ObjectPtr<TextureCube> skybox;
	};
}