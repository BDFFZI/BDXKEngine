#include "Cursor.h"
#include "BDXKEngine/Platform/Window/Window.h"

namespace BDXKEngine
{
    void Cursor::SetLockState(bool state)
    {
        Window::SetCursorLock(state);
    }

    void Cursor::SetVisible(bool state)
    {
        Window::SetCursorVisible(state);
    }
}
