#pragma once
#include "BDXKEngine/Platform/Window/Window.h"

namespace BDXKEngine {
	class Cursor
	{
	public:
		static void Initialize(Window* window);
		
		static void SetLockState(bool state);
		static void SetVisible(bool state);
	private:
		static Window* window;
		static int mouseButtonTime;
	};

}
