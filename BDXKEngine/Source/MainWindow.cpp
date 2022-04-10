#include "MainWindow.h"
#include "Assert.h"
#include "Graphics.h"

MainWindow::MainWindow() :WindowBase(L"MainWindow")
{
	painter = {};
}

MainWindow::~MainWindow()
{
	delete(painter);
}

LRESULT MainWindow::HandleMessage(UINT messageSign, WPARAM wparameter, LPARAM lparameter)
{
	switch (messageSign)
	{
	case WM_CREATE:
		Graphics::SetRenderTarget(hwnd);
		return 0;
	case WM_CLOSE:
		if (MessageBox(hwnd, L"ȷ���رգ�", L"�رմ���", MB_OKCANCEL) == IDOK)
			DestroyWindow(hwnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		OnPaint();
		return 0;
	case WM_SIZE:
		Graphics::ResetCanvas();
		return 0;
	}

	return DefWindowProc(hwnd, messageSign, wparameter, lparameter);
}

void MainWindow::OnPaint()
{
	Graphics::BeginDraw();

	Graphics::ClearCanvas(Color::Blue);
	Graphics::SetBrushColor(Color::Green);
	Graphics::DrawCircle(painter->GetCanvasSize()/2, 50, true);

	Graphics::EndDraw();
}
