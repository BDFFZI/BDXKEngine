#include "GUI.h"
#include <cmath>

Window* GUI::window;
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
	Color background = Event::IsDrag(rect) ? Color::yellow : Color::green;

	//可视化UI
	background.a -= 0.5f;
	GL2D::SetBrushColor(background);
	GL2D::DrawRectangle(rect, true);
	GL2D::SetBrushColor(Color::black);
	GL2D::DrawTextf(rect, text, FontSize);

	return Event::IsClick(rect);
}

std::wstring GUI::TextArea(Rect rect, std::wstring text, int fontSize)
{
	Color background;
	if (Event::IsDrag(rect))
		background = Color::gray;
	else if (Event::IsFocus(rect))//有焦点的时候接受文字输入
	{
		background = Color::lightBlue;
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
	}
	else background = Color::white;

	background.a -= 0.5f;
	GL2D::SetBrushColor(background);
	GL2D::DrawRectangle(rect, true);
	GL2D::SetBrushColor(Color::black);
	GL2D::DrawTextf(rect, text, fontSize);

	return text;
}



GUI GUI::Initialize(GL2D* graphics, Event* event, Window* window)
{
	GUI::window = window;
	window->AddMessageListener(OnWindowMessage);

	return {};
}

void GUI::OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter) {
	//文字输入
	if (Event::HasFocus()) {
		switch (messageSign)
		{
		case WM_CHAR:
			charStream << (wchar_t)wparameter;
		case WM_KEYDOWN:
			if ((KeyCode)wparameter == KeyCode::Backspace)
				deleteStream++;
		}
	}
	else
	{
		charStream.str(L"");
		deleteStream = 0;
	}
}
