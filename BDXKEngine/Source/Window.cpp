#include "Window.h"
#include "Graphics.h"

Window::Window(const wchar_t* name,
	std::function<void(Window* window)> onCreate,
	std::function<void(Window* window)> onPaint,
	std::function<void(Window* window)> onSize) :WindowBase(name)
{
	OnCreate = onCreate;
	OnPaint = onPaint;
	OnSize = onSize;
}

Window::Window() :Window(L"示例窗口",
	[&](Window* window) {
		Graphics::SetRenderTarget(window->GetHwnd());
	},
	[](Window* window) {
		Graphics::BeginDraw();

		Graphics::ClearCanvas(Color::Blue);
		Graphics::SetBrushColor(Color::Green);
		Graphics::DrawCircle(Graphics::GetCanvasSize() / 2, 50, true);

		Graphics::EndDraw();
	},
		[&](Window* window) {
		Graphics::ResetCanvas();
		window->RePaint();
	}) {
};

LRESULT Window::HandleMessage(UINT messageSign, WPARAM wparameter, LPARAM lparameter)
{
	switch (messageSign)
	{
	case WM_CREATE:
		OnCreate(this);
		return 0;
	case WM_CLOSE:
		if (MessageBox(hwnd, L"确定关闭？", L"关闭窗口", MB_OKCANCEL) == IDOK)
			DestroyWindow(hwnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		OnPaint(this);
		return 0;
	case WM_SIZE:
		OnSize(this);
		return 0;
	}

	return DefWindowProc(hwnd, messageSign, wparameter, lparameter);
}

void Window::RePaint(bool clear)
{
	InvalidateRect(hwnd, NULL, clear);
}
