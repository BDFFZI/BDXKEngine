#include "Cull.h"

namespace BDXKEngine
{
    Cull Cull::Default = {};

    Cull::Cull() = default;
    Cull::Cull(State state)
    {
        this->state = state;
    }
    void Cull::Transfer(Transferer& transferer)
    {
        TransferFieldInfoOf(state, int);
    }
}
