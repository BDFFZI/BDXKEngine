#pragma once
#include "Rect.h"
#include "Input.h"
#include "Component.h"

namespace BDXKEngine {
	class Event
	{
	public:
		static bool IsClick(Rect rect);
		static bool IsDrag(Rect rect, ObjectPtr<Component> dragSource);
		static bool IsDrag(Rect rect);
		static bool IsDrop(Rect rect, ObjectPtr<Component>* dragSource = nullptr);
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
		static ObjectPtr<Component> dragSource;

		static void MarkEvent(Rect rect);
	};
}
