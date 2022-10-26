#include "QualitySettings.h"

namespace BDXKEngine
{
    float QualitySettings::GetShadowDistance() const
    {
        return shadowDistance;
    }
    void QualitySettings::SetShadowDistance(float shadowDistance)
    {
        this->shadowDistance = shadowDistance;
    }
}
