#include "Graphics.h"
#include "Com.h"
#include "Assert.h"
#include "Window.h"

HWND Graphics::hwnd = NULL;
PAINTSTRUCT* Graphics::paintStruct = NULL;
CComPtr<ID2D1Factory> Graphics::factory = NULL;
CComPtr<ID2D1HwndRenderTarget> Graphics::renderTarget = NULL;
CComPtr<ID2D1SolidColorBrush> Graphics::brush = NULL;

void Graphics::SetRenderTarget(HWND hwnd)
{
	Graphics::hwnd = hwnd;
	Graphics::paintStruct = NULL;

	LRESULT back = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	Assert::IsSucceeded(back, "������Դ����ʧ��");

	CreateResources();
}

void Graphics::ResetCanvas()
{
	renderTarget->Resize(Window::GetSize(hwnd));
}

void Graphics::ClearCanvas(Color color)
{
	renderTarget->Clear(color);
}

Vector2 Graphics::GetCanvasSize()
{
	return renderTarget->GetSize();
}

void Graphics::SetBrushColor(Color color)
{
	brush->SetColor(color);
}

void Graphics::SetTransform(Matrix matrix)
{
	renderTarget->SetTransform(matrix);
}

void Graphics::BeginDraw(Color color)
{
	if (renderTarget == NULL)
		CreateResources();

	//Direct2D����Ҫ,��GDI��Ҫ�����ֻ���������ϵͳ���еģ���Ҫ����ϣ���ɾ
	BeginPaint(hwnd, paintStruct);
	renderTarget->BeginDraw();
	SetBrushColor(color);
	SetTransform(D2D1::IdentityMatrix());
}

void Graphics::EndDraw()
{
	LRESULT back = renderTarget->EndDraw();
	if (FAILED(back) || back == D2DERR_RECREATE_TARGET)
	{
		DeleteResources();
	}
	EndPaint(hwnd, paintStruct);
}

void Graphics::DrawCircle(Vector2 center, float radius, bool isFill)
{
	const D2D1_ELLIPSE ellipse = D2D1::Ellipse(center, radius, radius);
	if (isFill)
		renderTarget->FillEllipse(ellipse, brush);
	else
		renderTarget->DrawEllipse(ellipse, brush);
}

void Graphics::DrawRectangleCenter(Vector2 center, Vector2 size, bool isFill)
{
	DrawRectangle(center - size / 2, size, isFill);
}

void Graphics::DrawRectangle(Vector2 origin, Vector2 size, bool isFill)
{
	const D2D1_RECT_F rect = D2D1::RectF(origin.x, origin.y, origin.x + size.x,origin.y + size.y);
	if (isFill)
		renderTarget->FillRectangle(rect, brush);
	else
		renderTarget->DrawRectangle(rect, brush);
}

void Graphics::CreateResources()
{
	//����������Ŀ��
	RECT rect;
	GetClientRect(hwnd, &rect);
	D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);
	LRESULT back = factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hwnd, size),
		&renderTarget
	);
	Assert::IsSucceeded(back, "����������Ŀ��ʧ��");

	//��������
	D2D1_COLOR_F color = D2D1::ColorF(1, 1, 1);
	back = renderTarget->CreateSolidColorBrush(color, &brush);
	Assert::IsSucceeded(back, "��������ʧ��");
}

void Graphics::DeleteResources()
{
	renderTarget = NULL;
	brush = NULL;
}
