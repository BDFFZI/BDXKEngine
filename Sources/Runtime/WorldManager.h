#pragma once
#include "Window.h"
#include "GameObject.h"

namespace BDXKEngine {
	class WorldManager {
	protected:
		static WorldManager* Initialize(Window* window);
	};
}