#include "MainWindow.h"
#include "Assert.h"

MainWindow::MainWindow() :WindowBase(L"MainWindow")
{
	factory = NULL;
	renderTarget = NULL;
	brush = NULL;
}

LRESULT MainWindow::HandleMessage(UINT messageSign, WPARAM wparameter, LPARAM lparameter)
{
	switch (messageSign)
	{
	case WM_CREATE:
		Assert::IsSucceeded(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory), "������Դ����ʧ��");
		return 0;
	case WM_CLOSE:
		if (MessageBox(hwnd, L"ȷ���رգ�", L"�رմ���", MB_OKCANCEL) == IDOK)
			DestroyWindow(hwnd);
		return 0;
	case WM_DESTROY:
		DiscardGraphicsResources();
		factory = NULL;
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		OnPaint();
		return 0;
	case WM_SIZE:
		OnReSize();
		return 0;
	}

	return DefWindowProc(hwnd, messageSign, wparameter, lparameter);
}
LRESULT MainWindow::CreateGraphicsResources()
{
	LRESULT back = S_OK;

	if (renderTarget == NULL)
	{
		//����������Ŀ��
		RECT rect;
		GetClientRect(hwnd, &rect);
		D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);
		back = factory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(hwnd, size),
			&renderTarget
		);
		Assert::IsSucceeded(back, "����������Ŀ��ʧ��");

		//��������
		D2D1_COLOR_F color = D2D1::ColorF(0, 1, 0);
		back = renderTarget->CreateSolidColorBrush(color, &brush);
		Assert::IsSucceeded(back, "��������ʧ��");
	}

	return back;
}

void MainWindow::DiscardGraphicsResources()
{
	renderTarget = NULL;
	brush = NULL;
}

void MainWindow::OnReSize()
{
	if (renderTarget != NULL)
	{
		RECT rect;
		GetClientRect(hwnd, &rect);
		D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);

		renderTarget->Resize(size);
		//��С����ʱ���ᷢ���ػ��¼��������ֶ������ⲿ�ֻ��棬�Ա㼤���ػ��¼�
		InvalidateRect(hwnd, &rect, true);
	}
}

void MainWindow::OnPaint()
{
	LRESULT back = CreateGraphicsResources();
	if (SUCCEEDED(back))
	{
		//Direct2D����Ҫ,��GDI��Ҫ�����ֻ���������ϵͳ���еģ���Ҫ����ϣ���ɾ
		PAINTSTRUCT paintStruct;
		HDC hdc = BeginPaint(hwnd, &paintStruct);

		renderTarget->BeginDraw();

		//���ñ���
		renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
		//���û�����ɫ
		D2D1_COLOR_F color = brush->GetColor();
		color.r = fmod((color.r + 0.01f), 1);
		brush->SetColor(color);
		//����Բ��
		D2D1_SIZE_F size = renderTarget->GetSize();
		const float x = size.width / 2;
		const float y = size.height / 2;
		const float radius = fmin(x, y);
		const D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
		renderTarget->FillEllipse(ellipse, brush);

		back = renderTarget->EndDraw();
		if (FAILED(back) || back == D2DERR_RECREATE_TARGET)
		{
			DiscardGraphicsResources();
		}

		EndPaint(hwnd, &paintStruct);
	}
}
