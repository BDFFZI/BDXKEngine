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
	void Show();
	PCWSTR GetWindowName();
protected:
	virtual LRESULT CALLBACK HandleMessage(HWND hwnd, UINT messageSign, WPARAM wparameter, LPARAM lparameter);
	PCWSTR WindowName;
private:
	static LRESULT CALLBACK WindowProcess(HWND hwnd, UINT messageSign, WPARAM parameterA, LPARAM parameterB);
};


