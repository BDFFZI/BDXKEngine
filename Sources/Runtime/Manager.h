#pragma once
#include "Window.h"

namespace BDXKEngine {
	class Manager
	{
	protected:
		Manager(Window* window);

		virtual void OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter) = 0;
	};
}


