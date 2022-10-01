#pragma once
#include "BDXKEngine/Base/Rect.h"
#include "Input.h"
#include "BDXKEngine/Framework/Component/Component.h"

namespace BDXKEngine
{
    class Event
    {
    public:
        static bool IsClick(Rect rect);
        static bool IsDrag(Rect rect);
        static bool IsDrag(Rect rect, const ObjectPtrBase& dragSource);
        template <typename TObject>
        static bool IsDrop(Rect rect, ObjectPtr<TObject>* dropTarget = nullptr)
        {
            MarkEvent(rect);
            if (drop == rect)
            {
                if (dropTarget != nullptr && dynamic_cast<TObject*>(dragSource.ToObjectBase()) != nullptr)
                    *dropTarget = dragSource;
                return true;
            }
            return false;
        }
        static bool IsFocus(Rect rect);
        static void Use();

        static bool HasClick();
        static bool HasDrag();
        static bool HasFocus();
    protected:
        static void Initialize(Window* window);
    private:
        static Window* window;
        static Rect drag;
        static Rect drop;
        static Rect click;
        static Rect focus;
        static ObjectPtrBase dragSource;

        static void MarkEvent(Rect rect);
    };
}
