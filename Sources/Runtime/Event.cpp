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
				else throw std::exception("�¼���ͻ��ͬһλ��������������һ������ק�¼���");
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
	/// �ж��¼��ķ���
	/// ԭ��
	/// ���ͬһʱ��ֻ�ܴ���һ����Χ�С�
	/// ����걻���º󣬵ȴ�����ֻ����̧��
	/// �Դ˲�����һ������״̬
	/// </summary>
	void Event::MarkEvent(Rect rect)
	{
		//�����˵�����
		if (rect.Contains(window->GetCursorLocalPosition()) == false)
			return;

		if (Input::GetMouseButtonDown(0))
		{
			//����Ч��Χ�ڵ�����������¼����ȴ�̧�����ʱ��֤
			drag = rect;
		}
		else if (Input::GetMouseButtonUp(0))
		{
			//����¼�
			if (rect == drag)
			{
				focus = drag;
				click = drag;
			}
			//�����¼�
			else
			{
				drop = rect;
			}
		}
	}

	int mouseButtonUpFrame = -1;
	/// <summary>
	/// ���շ������¼�
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