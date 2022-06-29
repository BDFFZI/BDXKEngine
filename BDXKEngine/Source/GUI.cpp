#include "GUI.h"
#include <cmath>

Window* GUI::window;
Rect GUI::clicking = Rect::zero;
Rect GUI::clicked = Rect::zero;
std::wstringstream GUI::charStream{};
int GUI::deleteStream{};

int FontSize = 20;

//��֧�ֵ��У�Ч��Ҳ����
//int ComputeFontSize(Rect rect)
//{
//	float length = fmin(rect.width, rect.height);
//	return (int)(length / 30 * 20);
//}

bool GUI::Button(Rect rect, std::wstring text)
{
	//���ӻ�UI
	GL2D::SetBrushColor(rect == clicking ? Color::yellow : Color::green);
	GL2D::DrawRectangle(rect, true);
	GL2D::SetBrushColor(Color::black);
	GL2D::DrawTextf(rect, text, FontSize);

	return IsClick(rect);
}

std::wstring GUI::TextArea(Rect rect, std::wstring text)
{
	IsClick(rect);

	//���ӻ�UI
	if (rect == clicking)
		GL2D::SetBrushColor(Color::gray);
	else if (rect == clicked)//�н����ʱ�������������
	{
		//ɾ��
		if (deleteStream != 0)
		{
			if (text.length() != 0)
			{
				text.pop_back();
				deleteStream--;
			}
			else
			{
				deleteStream = 0;
			}
		}

		//����
		text.append(charStream.str().c_str());
		charStream.str(L"");


		GL2D::SetBrushColor(Color::lightBlue);
	}
	else
		GL2D::SetBrushColor(Color::white);
	GL2D::DrawRectangle(rect, true);
	GL2D::SetBrushColor(Color::black);
	GL2D::DrawTextf(rect, text, FontSize);

	return text;
}

//����¼�����
//ԭ��
//���ͬһʱ��ֻ�ܴ���һ����Χ�С�
//����걻���º󣬵ȴ�����ֻ����̧��
//�Դ˲�����һ������״̬
bool GUI::IsClick(Rect rect)
{
	if (rect.Contains(window->GetCursorLocalPosition()) && Input::GetMouseButtonDown(0))
	{
		//����Ч��Χ�ڵ�����������¼����ȴ�̧�����ʱ��֤
		clicking = rect;
	}
	else if (Input::GetMouseButtonUp(0)
		&& rect == clicking)//�����˵�����
	{
		//�����ԭλ��̧���¼���Ч
		if (clicking.Contains(window->GetCursorLocalPosition()))
		{
			clicked = clicking;
			clicking = Rect::zero;
			return true;
		}

		clicking = Rect::zero;
	}

	return false;
}

GUI GUI::Initialize(GL2D* graphics, Input* input, Window* window)
{
	GUI::window = window;
	window->AddMessageListener(OnWindowMessage);

	return {};
}

void GUI::OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter) {
	switch (messageSign)
	{
	case WM_LBUTTONDOWN:
		clicked = Rect::zero;
		charStream.str(L"");
		deleteStream = 0;
		break;
	}

	//��������
	if (clicked != Rect::zero) {
		switch (messageSign)
		{
		case WM_CHAR:
			charStream << (wchar_t)wparameter;
		case WM_KEYDOWN:
			if ((KeyCode)wparameter == KeyCode::Backspace)
				deleteStream++;
		}
	}
}
