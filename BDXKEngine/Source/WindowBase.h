#pragma once
#include <windows.h>
#include<stdexcept>
#include "Vector2Int.h"

class WindowBase {
public:
	WindowBase(PCWSTR name);
	void Show();
	PCWSTR GetName();
	HWND GetHwnd();
protected:
	virtual LRESULT CALLBACK HandleMessage(UINT messageSign, WPARAM wparameter, LPARAM lparameter);
	PCWSTR name;
	HWND hwnd;
private:
	static LRESULT CALLBACK WindowProcess(HWND hwnd, UINT messageSign, WPARAM parameterA, LPARAM parameterB);
};


