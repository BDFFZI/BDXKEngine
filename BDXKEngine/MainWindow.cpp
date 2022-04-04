#include <iostream>
using namespace std;
#include "MainWindow.h"

MainWindow::MainWindow() :WindowBase(L"MainWindow")
{
}

LRESULT MainWindow::HandleMessage(HWND hwnd, UINT messageSign, WPARAM wparameter, LPARAM lparameter)
{
	switch (messageSign)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_GRADIENTACTIVECAPTION));

		EndPaint(hwnd, &ps);
	}
	return 0;
	}

	return DefWindowProc(hwnd, messageSign, wparameter, lparameter);
}
