#include "GUI.h"
#include <cmath>

Window* GUI::window;
Rect GUI::clicking = Rect::zero;
Rect GUI::clicked = Rect::zero;
std::wstringstream GUI::charStream{};
int GUI::deleteStream{};

int FontSize = 20;

//仅支持单行，效果也不好
//int ComputeFontSize(Rect rect)
//{
//	float length = fmin(rect.width, rect.height);
//	return (int)(length / 30 * 20);
//}

bool GUI::Button(Rect rect, std::wstring text)
{
	//可视化UI
	GL2D::SetBrushColor(rect == clicking ? Color::yellow : Color::green);
	GL2D::DrawRectangle(rect, true);
	GL2D::SetBrushColor(Color::black);
	GL2D::DrawTextf(rect, text, FontSize);

	return IsClick(rect);
}

std::wstring GUI::TextArea(Rect rect, std::wstring text)
{
	IsClick(rect);

	//可视化UI
	if (rect == clicking)
		GL2D::SetBrushColor(Color::gray);
	else if (rect == clicked)//有焦点的时候接受文字输入
	{
		//删除
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

		//输入
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

//点击事件处理
//原理：
//鼠标同一时间只能处于一个范围中。
//当鼠标被按下后，等待它的只能是抬起。
//以此产生了一种锁定状态
bool GUI::IsClick(Rect rect)
{
	if (rect.Contains(window->GetCursorLocalPosition()) && Input::GetMouseButtonDown(0))
	{
		//在有效范围内点击，锁定该事件，等待抬起鼠标时验证
		clicking = rect;
	}
	else if (Input::GetMouseButtonUp(0)
		&& rect == clicking)//闲杂人等勿扰
	{
		//鼠标在原位置抬起，事件有效
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

	//文字输入
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
