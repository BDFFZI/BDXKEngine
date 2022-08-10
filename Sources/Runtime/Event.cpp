#include "Event.h"

namespace BDXKEngine {
	Rect Event::drag = Rect::zero;
	Rect Event::drop = Rect::zero;
	Rect Event::click = Rect::zero;
	Rect Event::focus = Rect::zero;
	Window* Event::window = nullptr;
	ObjectPtr<Component> Event::dragSource = nullptr;

	bool Event::IsClick(Rect rect)
	{
		MarkEvent(rect);
		return click == rect;
	}
	bool Event::IsDrag(Rect rect, ObjectPtr<Component> dragSource)
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
	bool Event::IsDrop(Rect rect, ObjectPtr<Component>* dragSource)
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

	void Event::Initialize(Window* window)
	{
		Event::window = window;
		window->AddRenewEvent([]() {
			if (Input::GetMouseButtonDown(0))
			{
				focus = Rect::zero;
			}
			//���շ������¼�
			if (Input::GetMouseButtonUp(0))
			{
				drag = Rect::zero;
				drop = Rect::zero;
				click = Rect::zero;
				dragSource = nullptr;
			}
			});
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
}