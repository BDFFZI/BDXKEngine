#pragma once
#include <windows.h>
#include<stdexcept>

class WindowBase {
public:
	WindowBase(PCWSTR windowName);
	void Show();
	PCWSTR GetWindowName();
protected:
	virtual LRESULT CALLBACK HandleMessage(UINT messageSign, WPARAM wparameter, LPARAM lparameter);
	PCWSTR windowName;
	HWND hwnd;
private:
	static LRESULT CALLBACK WindowProcess(HWND hwnd, UINT messageSign, WPARAM parameterA, LPARAM parameterB);
};


