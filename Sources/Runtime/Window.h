#pragma once
#include <functional>
#include <vector>
#include "WindowBase.h"
#include "Rect.h"


class Window : public WindowBase
{
#define MessageHandler std::function<LRESULT(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)>
#define MessageListener std::function<void(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)>

public:
	Window(const wchar_t* name, MessageHandler messageHandler);

	void AddMessageListener(MessageListener messageListener);

	Rect GetScreenRect();
	Vector2 GetSize();
	Vector2 GetCursorLocalPosition();
	Vector2 GetCursorMoveDelta();

	void ConfiningCursor(bool isOpen);
	void SetCursorLocalPosition(Vector2 localPosition);
	void SetCursorLock(bool state);

protected:
	void RePaint(bool clear = true);
	LRESULT HandleMessage(UINT messageSign, WPARAM wparameter, LPARAM lparameter) override;
private:
	MessageHandler messageHandler;
	std::vector<MessageListener> messageListeners;

	Vector2 lockCursorPos{};
	Vector2 lastCursorPos{};
	Vector2 cursorPos{};
	bool cursorlock{};

};

