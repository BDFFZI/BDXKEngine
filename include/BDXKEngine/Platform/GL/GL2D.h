#pragma once
#include <string>
#include <d2d1.h>
#include <dwrite.h>
#include <atlbase.h>
#include "GL.h"
#include "BDXKEngine/Base/Data/Mathematics/Vector/Vector2.h"
#include "BDXKEngine/Base/Color.h"
#include "BDXKEngine/Base/Matrix/Matrix3x2.h"
#include "BDXKEngine/Base/Rect.h"

namespace BDXKEngine {
	//缺乏维护，请用imGui代替
	class GL2D :protected GL
	{
	public:
		static void CreateResources();
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
		static void DrawTextf(Rect rect, std::string text, int fontSize);
	protected:
		/// 仅能使用GL的默认交换链贴图
		static void Initialize();
	private:

		static CComPtr<IDWriteFactory> writeFactory;//资源工厂
		static CComPtr<ID2D1Factory> factory;//资源工厂
		static CComPtr<ID2D1RenderTarget> renderTarget;//呈现器目标,如缓冲区纹理窗口等，此处为窗口
		static CComPtr<ID2D1SolidColorBrush> brush;//画笔
	};
}
