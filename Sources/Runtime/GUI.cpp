#include "GUI.h"
#include <cmath>
#include <algorithm>

namespace BDXKEngine {
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

	float GUI::Slider(Rect rect, float value, float leftValue, float rightValue)
	{
		Color fontColor = Color::green;
		if (Event::IsDrag(rect))
		{
			value += window->GetCursorMoveDelta().x / 200;
			value = std::clamp(value, leftValue, rightValue);
			fontColor = Color::blue;
		}

		GL2D::SetBrushColor(Color::white);
		GL2D::DrawRectangle(rect, true);
		GL2D::SetBrushColor(fontColor);
		GL2D::DrawTextf(rect, std::to_wstring(value), 20);

		return value;
	}



	GUI* GUI::Initialize(Event* event, Window* window)
	{
		GUI::window = window;
		window->AddCharacterEvent([](wchar_t character) {
			if (Event::HasFocus)
				charStream << character;
			});
		window->AddKeyCodeEvent([](KeyCode keyCode, bool state) {
			if (Event::HasFocus() && keyCode == KeyCode::Backspace)
				deleteStream++;
			});
		window->AddRenewEvent([]() {
			charStream.str(L"");
			deleteStream = 0;
			});

		return new GUI();
	}
}