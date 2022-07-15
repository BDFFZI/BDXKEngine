#pragma once
#include <string>
#include <d2d1.h>
#include <dwrite.h>
#include <atlbase.h>
#include "Vector2.h"
#include "Color.h"
#include "Matrix3x2.h"
#include "Rect.h"

namespace BDXKEngine {
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
		static void DrawRectangle(Rect rect, bool isFill);
		static void DrawRectangleCenter(Vector2 center, Vector2 size, bool isFill);
		static void DrawTextf(Rect rect, std::wstring text, int fontSize);
	protected:
		static GL2D* Initialize(CComPtr<IDXGISurface> renderTargetTexture);
	private:

		static CComPtr<IDWriteFactory> writeFactory;//资源工厂
		static CComPtr<ID2D1Factory> factory;//资源工厂
		static CComPtr<ID2D1RenderTarget> renderTarget;//呈现器目标,如缓冲区纹理窗口等，此处为窗口
		static CComPtr<ID2D1SolidColorBrush> brush;//画笔
	};
}
