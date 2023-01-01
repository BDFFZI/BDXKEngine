#include "Event.h"

namespace BDXKEngine
{
    Rect Event::drag = {};
    Rect Event::drop = {};
    Rect Event::click = {};
    Rect Event::focus = {};
    Window* Event::window = nullptr;
    ObjectPtrBase Event::dragSource = nullptr;

    bool Event::IsClick(Rect rect)
    {
        MarkEvent(rect);
        return click == rect;
    }
    bool Event::IsDrag(Rect rect, const ObjectPtrBase& dragSource)
    {
        if (IsDrag(rect))
        {
            if (Event::dragSource != dragSource)
            {
                if (Event::dragSource.IsNull())Event::dragSource = dragSource;
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
        window->AddRenewEvent([]()
        {
            if (Input::GetMouseButtonDown(0))
            {
                focus = Rect::zero;
            }
            //回收发生的事件
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
}
