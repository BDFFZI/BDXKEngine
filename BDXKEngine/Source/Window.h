#pragma once
#include "WindowBase.h"
#include <functional>

class Window : public WindowBase
{
public:
	Window(const wchar_t* name,
		std::function<bool(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)> messageEvent
	);

	Window();

	void RePaint(bool clear = true);
protected:
	LRESULT HandleMessage(UINT messageSign, WPARAM wparameter, LPARAM lparameter) override;
private:
	std::function<bool(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)> messageEvent;
};

