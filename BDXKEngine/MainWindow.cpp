#include <iostream>
using namespace std;

#include "MainWindow.h"
MainWindow::MainWindow() :WindowBase(L"MainWindow")
{
	cout << "MainWindow" << endl;
}

LRESULT MainWindow::HandleMessage(UINT messageSign, WPARAM wparameter, LPARAM lparameter)
{
	switch (messageSign)
	{
		cout << messageSign << endl;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		// All painting occurs here, between BeginPaint and EndPaint.
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_HOTLIGHT + 1));
		EndPaint(hwnd, &ps);
	}
	return 0;
	}
}
