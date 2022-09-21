#pragma once
#include "Platform/Window/Window.h"
#include "Framework/GameObject.h"

namespace BDXKEngine {
	class WorldManager {
	protected:
		static WorldManager* Initialize(Window* window);
	};
}