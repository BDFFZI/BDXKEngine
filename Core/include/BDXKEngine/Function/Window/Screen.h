#pragma once
#include "BDXKEngine/Platform/Window/Window.h"

namespace BDXKEngine {
	class Screen
	{
	public:
		static void Initialize(Window* window);
		
		static Vector2 GetSize();
	private:
		inline static Window* window{};
	};
}