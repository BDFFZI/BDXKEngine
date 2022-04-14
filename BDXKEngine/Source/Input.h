#pragma once
#include "Vector2.h"

enum KeyCode {

};

class BDXKEngine;
class Input {
	friend BDXKEngine;
public:
	static Vector2 mousePosition;

	static bool GetMouseButtonDown(int mouseButtonIndex);
	static bool GetMouseButton(int mouseButtonIndex);
	static bool GetMouseButtonUp(int mouseButtonIndex);
	static bool GetKeyDown(KeyCode keyCode);
	static bool GetKey(KeyCode keyCode);
	static bool GetKeyUp(KeyCode keyCode);
private:
	static void Update();

	static bool lastMouseButtonState[];
	static bool mouseButtonState[];
	static bool lastKeyboardState[];
	static bool keyboardState[];
};