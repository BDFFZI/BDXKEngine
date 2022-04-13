#pragma once
#include "WindowBase.h"
#include <functional>

class Window : public WindowBase
{
public:
	Window(const wchar_t* name,
		std::function<void(Window *window)> onCreate,
		std::function<void(Window *window)> onPaint,
		std::function<void(Window *window)> onSize
	);

	Window();

	void RePaint(bool clear = true);
protected:
	LRESULT HandleMessage(UINT messageSign, WPARAM wparameter, LPARAM lparameter) override;
private:
	std::function<void(Window *window)> OnCreate;
	std::function<void(Window *window)> OnPaint;
	std::function<void(Window *window)> OnSize;
};

