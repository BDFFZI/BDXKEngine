#pragma once
#include<functional>
#include "Vector2.h"
#include "Window.h"

namespace BDXKEngine {
	enum class KeyCode :int {
		Backspace = 0x08,
		Delete = 0x2E,
		Tab = 0x09,
		Return = 0x0D,
		Esc = 0x1B,
		Space = 0x20,
		Alpha0 = 0x30,
		Alpha1 = 0x31,
		Alpha2 = 0x32,
		Alpha3 = 0x33,
		Alpha4 = 0x34,
		Alpha5 = 0x35,
		Alpha6 = 0x36,
		Alpha7 = 0x37,
		Alpha8 = 0x38,
		Alpha9 = 0x39,
		A = 0x41,
		B = 0x42,
		C = 0x43,
		D = 0x44,
		E = 0x45,
		F = 0x46,
		G = 0x47,
		H = 0x48,
		I = 0x49,
		J = 0x4A,
		K = 0x4B,
		L = 0x4C,
		M = 0x4D,
		N = 0x4E,
		O = 0x4F,
		P = 0x50,
		Q = 0x51,
		R = 0x52,
		S = 0x53,
		T = 0x54,
		U = 0x55,
		V = 0x56,
		W = 0x57,
		X = 0x58,
		Y = 0x59,
		Z = 0x5A,
		Shift = 0x10,
	};

	class Input {
	public:
		static bool GetMouseButtonDown(int mouseButtonIndex);
		static bool GetMouseButton(int mouseButtonIndex);
		static bool GetMouseButtonUp(int mouseButtonIndex);
		static Vector2 GetMousePosition() { return mousePosition; };
		static float GetMouseScrollDelta() { return mouseScrollDelta; };
		static Vector2 GetMouseMoveDelta() { return window->GetCursorMoveDelta(); };
		static bool GetKeyDown(KeyCode keyCode);
		static bool GetKey(KeyCode keyCode);
		static bool GetKeyUp(KeyCode keyCode);
	protected:
		static Input* Initialize(Window* window);
		static void SetEnable(bool state);

	private:
		static Window* window;
		static float mouseScrollDelta;
		static Vector2 mousePosition;
		static bool mouseButtonState[3][3];
		static bool keyboardState[256][3];
		static bool enable;

		static void FlushState();
		static void OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter);
	};
}