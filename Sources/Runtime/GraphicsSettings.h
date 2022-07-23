#pragma once
#include "ObjectPtr.h"
#include "Shader.h"

namespace BDXKEngine {
	struct GraphicsSettings {
		inline static ObjectPtr<Shader> shadowShader;
	};
}