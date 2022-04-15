#pragma once
#include "WindowBase.h"
#include "Rect.h"
#include <functional>

class Window : public WindowBase
{
public:
	static Vector2Int GetSize(HWND hwnd);

	Window(const wchar_t* name,
		std::function<bool(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)> messageEvent
	);

	Window();

	Rect GetRect();
	Rect GetScreenRect();
	Vector2 GetSize();
	void ConfiningCursor(bool isOpen);
	void RePaint(bool clear = true);
protected:
	LRESULT HandleMessage(UINT messageSign, WPARAM wparameter, LPARAM lparameter) override;
private:
	std::function<bool(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)> messageEvent;
};

