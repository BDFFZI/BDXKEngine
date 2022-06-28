#pragma once
#include <functional>
#include "WindowBase.h"
#include "Rect.h"

class Window : public WindowBase
{
public:
	static Rect GetScreenRect(HWND hwnd);
	static Vector2 GetSize(HWND hwnd);
	static void ConfiningCursor(HWND hwnd, bool isOpen);
	static void RePaint(HWND hwnd, bool clear = true);
	static Vector2 GetCursorLocalPosition(HWND hwnd);
	static void SetCursorLocalPosition(HWND hwnd, Vector2 localPosition);
	static Vector2 GetCursorMoveDelta();
	static void SetCursorLock(HWND hwnd, bool state);

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

