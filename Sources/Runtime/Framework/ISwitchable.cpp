#include "ISwitchable.h"

namespace BDXKEngine
{
    bool ISwitchable::IsActivatingAndEnabling() const
    {
        return GetIsActivating() && GetIsEnabling();
    }
}
