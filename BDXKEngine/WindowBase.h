#pragma once
//PCWSTR = char
//LRESULT = int
//HWND = struct HWND__{int unused;}µÄÖ¸Õë
//BOOL = int
//WPARAM = uint
//LPARAM = int
#include <windows.h>

class WindowBase {
public:
	WindowBase(PCWSTR windowName);
	BOOL Show();
	PCWSTR GetWindowName();
	HWND GetHWND();
protected:
	virtual LRESULT HandleMessage(UINT messageSign, WPARAM wparameter, LPARAM lparameter);
	PCWSTR WindowName;
	HWND hwnd;
private:
	static LRESULT CALLBACK WindowProcess(HWND hwnd, UINT messageSign, WPARAM parameterA, LPARAM parameterB);
};


