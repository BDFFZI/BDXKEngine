#pragma once
#include "BDXKEngine/Framework/Object/ScriptableObject.h"

namespace BDXKEngine
{
    class QualitySettings : public ScriptableObject
    {
    public:
        float GetShadowDistance() const;
        void SetShadowDistance(float shadowDistance);
    private:
        float shadowDistance = 150;
    };
}
