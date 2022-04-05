#include "MainWindow.h"
#include "Assert.h"

MainWindow::MainWindow() :WindowBase(L"MainWindow")
{
	factory = NULL;
	renderTarget = NULL;
	brush = NULL;
}

LRESULT MainWindow::HandleMessage(UINT messageSign, WPARAM wparameter, LPARAM lparameter)
{
	switch (messageSign)
	{
	case WM_CREATE:
		Assert::IsSucceeded(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory), "创建资源工厂失败");
		return 0;
	case WM_CLOSE:
		if (MessageBox(hwnd, L"确定关闭？", L"关闭窗口", MB_OKCANCEL) == IDOK)
			DestroyWindow(hwnd);
		return 0;
	case WM_DESTROY:
		DiscardGraphicsResources();
		factory = NULL;
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		OnPaint();
		return 0;
	case WM_SIZE:
		OnReSize();
		return 0;
	}

	return DefWindowProc(hwnd, messageSign, wparameter, lparameter);
}
LRESULT MainWindow::CreateGraphicsResources()
{
	LRESULT back = S_OK;

	if (renderTarget == NULL)
	{
		//创建呈现器目标
		RECT rect;
		GetClientRect(hwnd, &rect);
		D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);
		back = factory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(hwnd, size),
			&renderTarget
		);
		Assert::IsSucceeded(back, "创建呈现器目标失败");

		//创建画笔
		D2D1_COLOR_F color = D2D1::ColorF(0, 1, 0);
		back = renderTarget->CreateSolidColorBrush(color, &brush);
		Assert::IsSucceeded(back, "创建画笔失败");
	}

	return back;
}

void MainWindow::DiscardGraphicsResources()
{
	renderTarget = NULL;
	brush = NULL;
}

void MainWindow::OnReSize()
{
	if (renderTarget != NULL)
	{
		RECT rect;
		GetClientRect(hwnd, &rect);
		D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);

		renderTarget->Resize(size);
		//缩小窗口时不会发送重绘事件，所以手动废弃这部分画面，以便激活重绘事件
		InvalidateRect(hwnd, &rect, true);
	}
}

void MainWindow::OnPaint()
{
	LRESULT back = CreateGraphicsResources();
	if (SUCCEEDED(back))
	{
		//Direct2D不需要,但GDI需要。部分绘制任务是系统进行的，需要打配合，别删
		PAINTSTRUCT paintStruct;
		HDC hdc = BeginPaint(hwnd, &paintStruct);

		renderTarget->BeginDraw();

		//设置背景
		renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
		//设置画笔颜色
		D2D1_COLOR_F color = brush->GetColor();
		color.r = fmod((color.r + 0.01f), 1);
		brush->SetColor(color);
		//绘制圆形
		D2D1_SIZE_F size = renderTarget->GetSize();
		const float x = size.width / 2;
		const float y = size.height / 2;
		const float radius = fmin(x, y);
		const D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
		renderTarget->FillEllipse(ellipse, brush);

		back = renderTarget->EndDraw();
		if (FAILED(back) || back == D2DERR_RECREATE_TARGET)
		{
			DiscardGraphicsResources();
		}

		EndPaint(hwnd, &paintStruct);
	}
}
