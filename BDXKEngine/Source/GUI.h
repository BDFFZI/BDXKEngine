#pragma once
#include <string>
#include <functional>
#include "Rect.h"
#include "GL2D.h"
#include "Input.h"

class GUI
{
public:
	static bool Button(Rect rect, std::wstring text);
	static std::wstring TextArea(Rect rect, std::wstring text);
	static bool IsClick(Rect rect);
protected:
	static GUI Initialize(GL2D* graphics, Input* input, Window* window);
private:
	static Window* window;
	static Rect clicking;
	static Rect clicked;
	static std::wstringstream charStream;
	static int deleteStream;

	static void OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter);
};

