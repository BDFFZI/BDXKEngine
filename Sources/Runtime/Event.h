#pragma once
#include "Rect.h"
#include "Window.h"
#include "Input.h"
#include "Component.h"

class Event
{
public:
	static bool IsClick(Rect rect);
	static bool IsDrag(Rect rect, Component* dragSource);
	static bool IsDrag(Rect rect);
	static bool IsDrop(Rect rect, Component** dragSource = nullptr);
	static bool IsFocus(Rect rect);
	static void Use();

	static bool HasClick();
	static bool HasDrag();
	static bool HasFocus();
protected:
	static Event* Initialize(Input* input, Window* window);
private:
	static Window* window;
	static Rect drag;
	static Rect drop;
	static Rect click;
	static Rect focus;
	static Component* dragSource;

	static void MarkEvent(Rect rect);
	static void OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter);
};

