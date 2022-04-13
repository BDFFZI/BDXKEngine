#pragma once
#include <d2d1.h>
#include "Com.h"
#include "Vector2.h"
#include "Color.h"
#include "Matrix.h"

class Graphics
{
public:
	static void SetRenderTarget(HWND hwnd);

	static void ResetCanvas();
	static void ClearCanvas(Color color = Color::Black);
	static Vector2 GetCanvasSize();

	static void SetBrushColor(Color color = Color::White);
	static void SetTransform(Matrix matrix = Matrix::Identity);

	static void BeginDraw(Color color = Color::White);
	static void EndDraw();

	static void DrawCircle(Vector2 center, float radius, bool isFill);
	static void DrawRectangle(Vector2 origin, Vector2 size, bool isFill);

	static ID2D1HwndRenderTarget* GetRenderTarget() { return renderTarget.p; }
private:
	static HWND hwnd;
	static PAINTSTRUCT* paintStruct;
	static CComPtr<ID2D1Factory> factory;//资源工厂
	static CComPtr<ID2D1HwndRenderTarget> renderTarget;//呈现器目标,如缓冲区纹理窗口等，此处为窗口
	static CComPtr<ID2D1SolidColorBrush> brush;//画笔

	static void CreateResources();
	static void DeleteResources();
};

