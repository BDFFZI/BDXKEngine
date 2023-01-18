#include "System.h"

namespace HelloGame
{
    void System::OnAwake()
    {
        GameObject::Hide(GetGameObject());
    }
}
