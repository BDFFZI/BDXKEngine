#include "Event.h"
#include "Debug.h"

namespace BDXKEngine {
	Rect Event::drag = Rect::zero;
	Rect Event::drop = Rect::zero;
	Rect Event::click = Rect::zero;
	Rect Event::focus = Rect::zero;
	Component* Event::dragSource = nullptr;
	Window* Event::window = nullptr;

	bool Event::IsClick(Rect rect)
	{
		MarkEvent(rect);
		return click == rect;
	}
	bool Event::IsDrag(Rect rect, Component* dragSource)
	{
		if (IsDrag(rect))
		{
			if (Event::dragSource != dragSource)
			{
				if (Event::dragSource == nullptr)Event::dragSource = dragSource;
				else throw std::exception("事件冲突，同一位置设置了两个不一样的拖拽事件。");
			}
			return true;
		}
		return false;
	}
	bool Event::IsDrag(Rect rect)
	{
		MarkEvent(rect);
		return drag == rect;
	}
	bool Event::IsDrop(Rect rect, Component** dragSource)
	{
		MarkEvent(rect);
		if (drop == rect)
		{
			if (dragSource != nullptr)*dragSource = Event::dragSource;
			return true;
		}
		return false;
	}
	bool Event::IsFocus(Rect rect)
	{
		MarkEvent(rect);
		return focus == rect;
	}

	void Event::Use()
	{
		drag = Rect::zero;
		drop = Rect::zero;
		click = Rect::zero;
		focus = Rect::zero;
		dragSource = nullptr;
	}

	bool Event::HasClick()
	{
		return click != Rect::zero;
	}
	bool Event::HasDrag()
	{
		return drag != Rect::zero;
	}
	bool Event::HasFocus()
	{
		return focus != Rect::zero;
	}

	Event* Event::Initialize(Input* input, Window* window)
	{
		Event::window = window;
		window->AddMessageListener(OnWindowMessage);

		return new Event();
	}

	/// <summary>
	/// 判断事件的发生
	/// 原理：
	/// 鼠标同一时间只能处于一个范围中。
	/// 当鼠标被按下后，等待它的只能是抬起。
	/// 以此产生了一种锁定状态
	/// </summary>
	void Event::MarkEvent(Rect rect)
	{
		//闲杂人等勿扰
		if (rect.Contains(window->GetCursorLocalPosition()) == false)
			return;

		if (Input::GetMouseButtonDown(0))
		{
			//在有效范围内点击，锁定该事件，等待抬起鼠标时验证
			drag = rect;
		}
		else if (Input::GetMouseButtonUp(0))
		{
			//点击事件
			if (rect == drag)
			{
				focus = drag;
				click = drag;
			}
			//落下事件
			else
			{
				drop = rect;
			}
		}
	}

	int mouseButtonUpFrame = -1;
	/// <summary>
	/// 回收发生的事件
	/// </summary>
	void Event::OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter) {
		if (messageSign == WM_PAINT)
		{
			if (mouseButtonUpFrame > -1)mouseButtonUpFrame--;
			if (mouseButtonUpFrame == 0) {
				drag = Rect::zero;
				drop = Rect::zero;
				click = Rect::zero;
				dragSource = nullptr;
			};

			if (Input::GetMouseButtonDown(0))
			{
				focus = Rect::zero;
			}
			if (Input::GetMouseButtonUp(0) || Input::GetMouseButtonUp(1))
				mouseButtonUpFrame = 1;
		}
	}
}