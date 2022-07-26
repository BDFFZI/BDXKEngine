#include "GL2D.h"
#include <assert.h>
#include "Texture2D.h"

namespace BDXKEngine {
	CComPtr<IDWriteFactory> GL2D::writeFactory = nullptr;
	CComPtr<ID2D1Factory> GL2D::factory = nullptr;
	CComPtr<ID2D1RenderTarget> GL2D::renderTarget = nullptr;
	CComPtr<ID2D1SolidColorBrush> GL2D::brush = nullptr;

	void GL2D::ClearCanvas(Color color)
	{
		renderTarget->Clear(color);
	}

	Vector2 GL2D::GetCanvasSize()
	{
		return renderTarget->GetSize();
	}

	void GL2D::SetBrushColor(Color color)
	{
		brush->SetColor(color);
	}

	void GL2D::SetMatrix(Matrix3x2 matrix)
	{
		renderTarget->SetTransform(matrix);
	}

	void GL2D::BeginDraw(Color color)
	{
		renderTarget->BeginDraw();
		SetBrushColor(color);
		SetMatrix(D2D1::IdentityMatrix());
	}

	void GL2D::EndDraw()
	{
		LRESULT back = renderTarget->EndDraw();
	}

	void GL2D::DrawCircle(Vector2 center, float radius, bool isFill)
	{
		const D2D1_ELLIPSE ellipse = D2D1::Ellipse(center, radius, radius);
		if (isFill)
			renderTarget->FillEllipse(ellipse, brush);
		else
			renderTarget->DrawEllipse(ellipse, brush);
	}

	void GL2D::DrawRectangleCenter(Vector2 center, Vector2 size, bool isFill)
	{
		DrawRectangle(center - size / 2, size, isFill);
	}

	void GL2D::DrawTextf(Rect rect, std::wstring text, int fontSize)
	{
		IDWriteTextFormat* textFormat;
		LRESULT result = writeFactory->CreateTextFormat(L"Arial", NULL,
			DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
			(float)fontSize, L"zh-cn", &textFormat);
		assert(SUCCEEDED(result));//字体格式创建失败

		D2D1_RECT_F d2d1Rect{ rect.x,rect.y,rect.GetXMax(),rect.GetYMax() };
		renderTarget->DrawTextW(text.c_str(), (UINT32)text.size(), textFormat, d2d1Rect, brush);

		textFormat->Release();
	}

	GL2D* GL2D::Initialize(GL* gl)
	{
		LRESULT back = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
		assert(SUCCEEDED(back));// 创建资源工厂失败
		CreateResources();

		back = DWriteCreateFactory(DWRITE_FACTORY_TYPE_ISOLATED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&writeFactory));
		assert(SUCCEEDED(back));// 创建写字资源工厂失败

		return new GL2D{};
	}

	void GL2D::DrawRectangle(Vector2 origin, Vector2 size, bool isFill)
	{
		const D2D1_RECT_F rect = D2D1::RectF(origin.x, origin.y, origin.x + size.x, origin.y + size.y);
		if (isFill)
			renderTarget->FillRectangle(rect, brush);
		else
			renderTarget->DrawRectangle(rect, brush);
	}

	void GL2D::DrawRectangle(Rect rect, bool isFill)
	{
		DrawRectangle(rect.GetPosition(), rect.GetSize(), isFill);
	}

	void GL2D::CreateResources()
	{
		HRESULT result = 0;

		//使用窗口作为渲染目标
		//RECT rect;
		//GetClientRect(hwnd, &rect);
		//D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);
		//LRESULT back = factory->CreateHwndRenderTarget(
		//	D2D1::RenderTargetProperties(),
		//	D2D1::HwndRenderTargetProperties(hwnd, size),
		//	&renderTarget
		//);
		//assert(SUCCEEDED(back));//创建呈现器目标失败

		//获取GL的默认渲染目标的DXGI底层资源
		CComPtr<IDXGISurface> dxgiSurface = nullptr;
		result = GL::GetDefaultRenderTarget()->QueryInterface(&dxgiSurface.p);
		assert(SUCCEEDED(result));

		//创建渲染目标
		D2D1_RENDER_TARGET_PROPERTIES properties = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)
		);
		result = factory->CreateDxgiSurfaceRenderTarget(dxgiSurface, &properties, &renderTarget.p);
		assert(SUCCEEDED(result));

		//创建画笔
		D2D1_COLOR_F color = D2D1::ColorF(1, 1, 1);
		assert(SUCCEEDED(renderTarget->CreateSolidColorBrush(color, &brush)));
	}
	void GL2D::ReleaseResources()
	{
		renderTarget = nullptr;
		brush = nullptr;
	}
}