#pragma once
#include <string>
#include <functional>
#include "Rect.h"
#include "GL2D.h"
#include "Input.h"
#include "Event.h"

namespace BDXKEngine {
	class GUI
	{
	public:
		static bool Button(Rect rect, std::wstring text);
		static std::wstring TextArea(Rect rect, std::wstring text, int fontSize = 20);
		static float Slider(Rect rect, float value, float leftValue, float rightValue);
		template<typename TValue>
		static void Array(Rect rect, TValue* array, int count)
		{
			std::wstringstream stream{};
			stream << '{';
			for (int i = 0; i < count; i++)
				stream << std::to_wstring(*(array + i)) << ',';
			stream << '}';

			TextArea(rect, stream.str());
		}
	protected:
		static GUI* Initialize(GL2D* graphics, Event* event, Window* window);
	private:
		static Window* window;
		static std::wstringstream charStream;
		static int deleteStream;

		static void OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter);
	};
}

