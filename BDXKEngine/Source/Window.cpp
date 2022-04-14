#include "Window.h"
#include "Graphics.h"

Window::Window(const wchar_t* name,
	std::function<bool(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)> messageEvent
) :WindowBase(name)
{
	this->messageEvent = messageEvent;
}

Window::Window() : Window(L"示例窗口",
	[](Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter) {
		switch (messageSign)
		{
		case WM_CREATE:
		{
			Graphics::SetRenderTarget(window->GetHwnd());
			return true;
		}
		case WM_CLOSE:
		{
			if (MessageBox(window->GetHwnd(), L"确定关闭？", L"关闭窗口", MB_OKCANCEL) == IDOK)
				DestroyWindow(window->GetHwnd());
			return true;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return true;
		}
		case WM_PAINT:
		{
			Graphics::BeginDraw();

			Graphics::ClearCanvas(Color::blue);
			Graphics::SetBrushColor(Color::green);
			Graphics::DrawCircle(Graphics::GetCanvasSize() / 2, 50, true);

			Graphics::EndDraw();
			return true;
		}
		case WM_SIZE:
		{
			Graphics::ResetCanvas();
			window->RePaint();
			return true;
		}
		}
		return false;
	}
) {
	return;
};

LRESULT Window::HandleMessage(UINT messageSign, WPARAM wparameter, LPARAM lparameter)
{
	if (messageEvent(this, messageSign, wparameter, lparameter))
		return 0;
	else
		return DefWindowProc(hwnd, messageSign, wparameter, lparameter);
}

void Window::RePaint(bool clear)
{
	InvalidateRect(hwnd, NULL, clear);
}
