#pragma once
#include "BDXKEngine/Platform/Window/Window.h"

namespace BDXKEngine {
	class Input {
	public:
		static void Initialize(Window* window);

		static bool GetMouseButtonDown(int mouseButtonIndex);
		static bool GetMouseButton(int mouseButtonIndex);
		static bool GetMouseButtonUp(int mouseButtonIndex);
		static Vector2 GetMousePosition() { return mousePosition; }
		static Vector2 GetMouseScrollDelta() { return mouseScrollDelta; }
		static Vector2 GetMouseMoveDelta();
		static bool GetKeyDown(KeyCode keyCode);
		static bool GetKey(KeyCode keyCode);
		static bool GetKeyUp(KeyCode keyCode);
		
		static void SetIsEnabling(bool state);
	private:
		static Window* window;
		static Vector2 mouseScrollDelta;
		static Vector2 mousePosition;
		static bool mouseButtonState[3][3];
		static bool keyboardState[256][3];
		static bool enable;
	};
}