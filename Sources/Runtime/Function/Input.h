#pragma once
#include "Platform/Window/Window.h"

namespace BDXKEngine {
	class Input {
	public:
		static bool GetMouseButtonDown(int mouseButtonIndex);
		static bool GetMouseButton(int mouseButtonIndex);
		static bool GetMouseButtonUp(int mouseButtonIndex);
		static Vector2 GetMousePosition() { return mousePosition; };
		static Vector2 GetMouseScrollDelta() { return mouseScrollDelta; };
		static Vector2 GetMouseMoveDelta() { return window->GetCursorMoveDelta(); };
		static bool GetKeyDown(KeyCode keyCode);
		static bool GetKey(KeyCode keyCode);
		static bool GetKeyUp(KeyCode keyCode);
	protected:
		static void Initialize(Window* window);
	private:
		static Window* window;
		static Vector2 mouseScrollDelta;
		static Vector2 mousePosition;
		static bool mouseButtonState[3][3];
		static bool keyboardState[256][3];
	};
}