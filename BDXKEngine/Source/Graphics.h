#pragma once
#include <d2d1.h>
#include "Com.h"
#include "Vector2.h"
#include "Color.h"

class Graphics
{
public:
	static void SetRenderTarget(HWND hwnd);

	static void ResetCanvas();
	static void ClearCanvas(Color color = Color::Black);
	static Vector2 GetCanvasSize();

	static void SetBrushColor(Color color);

	static void BeginDraw(Color color = Color::White);
	static void EndDraw();

	static void DrawCircle(Vector2 center, float radius, bool isFill);
	static void DrawRectangle(Vector2 origin, Vector2 size, bool isFill);

	static ID2D1HwndRenderTarget* GetRenderTarget() { return renderTarget.p; }
private:
	static HWND hwnd;
	static PAINTSTRUCT* paintStruct;
	static CComPtr<ID2D1Factory> factory;//��Դ����
	static CComPtr<ID2D1HwndRenderTarget> renderTarget;//������Ŀ��,�绺���������ڵȣ��˴�Ϊ����
	static CComPtr<ID2D1SolidColorBrush> brush;//����

	static void CreateResources();
	static void DeleteResources();
};

