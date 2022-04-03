#pragma once
#include "WindowBase.h"
class MainWindow : public WindowBase
{
public:
	MainWindow();

	LRESULT HandleMessage(UINT messageSign, WPARAM wparameter, LPARAM lparameter);
};

