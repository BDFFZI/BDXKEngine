#pragma once
#include "WindowBase.h"
class MainWindow : public WindowBase
{
public:
	MainWindow();

	LRESULT HandleMessage(HWND hwnd, UINT messageSign, WPARAM wparameter, LPARAM lparameter) override;
};

