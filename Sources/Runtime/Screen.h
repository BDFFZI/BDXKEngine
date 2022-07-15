#pragma once
#include <functional>
#include "Window.h"

namespace BDXKEngine {
	class Screen
	{
	public:
		static Vector2 GetSize();
	protected:
		static void Initialize(Window* window);

	private:
		inline static Window* window{};
	};
}