#include "GL2D.h"
#include <assert.h>

CComPtr<ID2D1Factory> GL2D::factory = NULL;
CComPtr<ID2D1RenderTarget> GL2D::renderTarget = NULL;
CComPtr<ID2D1SolidColorBrush> GL2D::brush = NULL;

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

void GL2D::Initialize(CComPtr<IDXGISurface> renderTargetTexture)
{
	LRESULT back = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	assert(SUCCEEDED(back));// 创建资源工厂失败

	CreateResources(renderTargetTexture);
}

void GL2D::DrawRectangle(Vector2 origin, Vector2 size, bool isFill)
{
	const D2D1_RECT_F rect = D2D1::RectF(origin.x, origin.y, origin.x + size.x, origin.y + size.y);
	if (isFill)
		renderTarget->FillRectangle(rect, brush);
	else
		renderTarget->DrawRectangle(rect, brush);
}

void GL2D::CreateResources(CComPtr<IDXGISurface> renderTargetTexture)
{
	//使用窗口作为呈现器
	//RECT rect;
	//GetClientRect(hwnd, &rect);
	//D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);
	//LRESULT back = factory->CreateHwndRenderTarget(
	//	D2D1::RenderTargetProperties(),
	//	D2D1::HwndRenderTargetProperties(hwnd, size),
	//	&renderTarget
	//);
	//assert(SUCCEEDED(back));//创建呈现器目标失败

	//利用纹理创建呈现器目标
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);
	assert(SUCCEEDED(factory->CreateDxgiSurfaceRenderTarget(renderTargetTexture, &props, &renderTarget.p)));

	//创建画笔
	D2D1_COLOR_F color = D2D1::ColorF(1, 1, 1);
	assert(SUCCEEDED(renderTarget->CreateSolidColorBrush(color, &brush)));
}
void GL2D::ReleaseResources()
{
	renderTarget = NULL;
	brush = NULL;
}
