#pragma once
#include<cmath>
#include<d2d1.h>
#include<stdexcept>
#include "ComUtility.h"
#include "WindowBase.h"
class MainWindow : public WindowBase
{
public:
	MainWindow();
protected:
	LRESULT HandleMessage(UINT messageSign, WPARAM wparameter, LPARAM lparameter) override;
	LRESULT CreateGraphicsResources();
	void DiscardGraphicsResources();
	void OnReSize();
	void OnPaint();
private:
	CComPtr<ID2D1Factory> factory;//资源工厂
	CComPtr<ID2D1HwndRenderTarget> renderTarget;//呈现器目标,如缓冲区纹理窗口等，此处为窗口
	CComPtr<ID2D1SolidColorBrush> brush;//画笔
};

