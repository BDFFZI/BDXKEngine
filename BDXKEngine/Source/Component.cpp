#include "Component.h"

String Component::ToString()
{
    return typeid(*this).name();
}
