#pragma once
#include<cmath>
#include<d2d1.h>
#include<stdexcept>
#include "Com.h"
#include "WindowBase.h"
#include "Graphics.h"

class MainWindow : public WindowBase
{
public:
	MainWindow();
	~MainWindow();
protected:
	LRESULT HandleMessage(UINT messageSign, WPARAM wparameter, LPARAM lparameter) override;
	void RePaint(bool clear = true);
private:
	void OnPaint();
	Graphics* painter;
};

