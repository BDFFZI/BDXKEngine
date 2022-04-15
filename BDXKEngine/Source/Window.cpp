#include "Window.h"
#include "Graphics.h"

Vector2Int Window::GetSize(HWND hwnd)
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);

	return size;
}

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
}

Rect Window::GetRect()
{
	RECT rect;
	GetClientRect(hwnd, &rect);
	D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);

	return rect;
}

Rect Window::GetScreenRect()
{
	Rect rect = GetRect();
	POINT min = rect.GetMin();
	POINT max = rect.GetMax();
	ClientToScreen(hwnd, &min);
	ClientToScreen(hwnd, &max);

	return Rect{ min ,max };
}

Vector2 Window::GetSize()
{
	Rect rect = GetRect();
	return rect.size;
}

void Window::ConfiningCursor(bool isOpen)
{
	if (isOpen)
	{
		RECT rect = GetScreenRect();
		ClipCursor(&rect);
	}
	else
	{
		ClipCursor(NULL);
	}
}

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
