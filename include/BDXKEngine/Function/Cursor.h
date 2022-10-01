#pragma once
#include "BDXKEngine/Platform/Window/Window.h"

namespace BDXKEngine {
	class Cursor
	{
	public:
		static void SetLockState(bool value);
		static void SetVisible(bool value);
	protected:
		static void Initialize(Window* window);
	private:
		static Window* window;
		static int mouseButtonTime;
	};

}
