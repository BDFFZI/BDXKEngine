#pragma once
#include <functional>
#include "WindowBase.h"
#include "Rect.h"

class Window : public WindowBase
{
public:
	static Rect GetRect(HWND hwnd);
	static Rect GetScreenRect(HWND hwnd);
	static Vector2 GetSize(HWND hwnd);
	static void ConfiningCursor(HWND hwnd, bool isOpen);
	static void RePaint(HWND hwnd, bool clear = true);
	static Vector2 GetCursorPos();
	static Vector2 GetCursorMoveDelta();
	static void SetCursorLock(bool state);

	Window(const wchar_t* name,
		std::function<LRESULT(HWND window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)> messageEvent
	);

protected:
	LRESULT HandleMessage(UINT messageSign, WPARAM wparameter, LPARAM lparameter) override;
private:
	std::function<LRESULT(HWND window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)> messageEvent;

	inline static Vector2 lockCursorPos{};
	inline static Vector2 lastCursorPos{};
	inline static Vector2 cursorPos{};
	inline static bool cursorlock{};

};

