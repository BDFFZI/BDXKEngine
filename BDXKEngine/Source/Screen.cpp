#include "Screen.h"
#include "Com.h"

Window* Screen::window;

void Screen::Initialize(Window* window) {
	Screen::window = window;
}
