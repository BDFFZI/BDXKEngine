#include "Screen.h"

Window* Screen::window;

void Screen::Initialize(Window* window) {
	Screen::window = window;
}
