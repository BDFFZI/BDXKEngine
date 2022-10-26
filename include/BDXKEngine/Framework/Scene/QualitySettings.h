#pragma once
#include "BDXKEngine/Framework/Object/SwitchableObject.h"

namespace BDXKEngine
{
    class QualitySettings : public SwitchableObject
    {
    public:
        float GetShadowDistance() const;
        void SetShadowDistance(float shadowDistance);
    private:
        float shadowDistance = 150;
    };
}
