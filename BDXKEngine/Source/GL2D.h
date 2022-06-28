#pragma once
#include <d2d1.h>
#include <atlbase.h>
#include "Vector2.h"
#include "Color.h"
#include "Matrix3x2.h"

class GL2D
{
public:
	static void CreateResources(CComPtr<IDXGISurface> renderTargetTexture);
	static void ReleaseResources();

	static void ClearCanvas(Color color = Color::black);
	static Vector2 GetCanvasSize();

	static void SetBrushColor(Color color = Color::white);
	static void SetMatrix(Matrix3x2 matrix = Matrix3x2::identity);

	static void BeginDraw(Color color = Color::white);
	static void EndDraw();

	static void DrawCircle(Vector2 center, float radius, bool isFill);
	static void DrawRectangle(Vector2 origin, Vector2 size, bool isFill);
	static void DrawRectangleCenter(Vector2 center, Vector2 size, bool isFill);
protected:
	static void Initialize(CComPtr<IDXGISurface> renderTargetTexture);
private:
	static CComPtr<ID2D1Factory> factory;//��Դ����
	static CComPtr<ID2D1RenderTarget> renderTarget;//������Ŀ��,�绺���������ڵȣ��˴�Ϊ����
	static CComPtr<ID2D1SolidColorBrush> brush;//����
};

