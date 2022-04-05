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
	CComPtr<ID2D1Factory> factory;//��Դ����
	CComPtr<ID2D1HwndRenderTarget> renderTarget;//������Ŀ��,�绺���������ڵȣ��˴�Ϊ����
	CComPtr<ID2D1SolidColorBrush> brush;//����
};

