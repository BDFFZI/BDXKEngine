#pragma once
#include <windows.h>
#include<stdexcept>
#include "Vector2Int.h"
#include "WindowUtility.h"

class WindowBase {
public:
	WindowBase(PCWSTR name);
	void Show();
	PCWSTR GetName();
	HWND GetHwnd();
	Vector2Int GetSize();
protected:
	virtual LRESULT CALLBACK HandleMessage(UINT messageSign, WPARAM wparameter, LPARAM lparameter);
	PCWSTR name;
	HWND hwnd;
	Vector2Int size;
private:
	static LRESULT CALLBACK WindowProcess(HWND hwnd, UINT messageSign, WPARAM parameterA, LPARAM parameterB);
};


