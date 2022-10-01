#pragma once
#include "BDXKEngine/Base/Vector/Vector2.h"

namespace BDXKEngine
{
    struct Rect
    {
        const static Rect zero;
        
        float x;
        float y;
        float width;
        float height;

        Rect();
        Rect(float x, float y, float width, float height);
        Rect(Vector2 min, Vector2 max);
        
        Vector2 GetPosition();
        Vector2 GetSize();
        Vector2 GetMin();
        Vector2 GetMax() const;
        float GetXMin() const;
        float GetYMin() const;
        float GetXMax() const;
        float GetYMax() const;
        void SetSize(Vector2 size);
        void SetPosition(Vector2 position);
        
        bool Contains(Vector2 position) const;

        bool operator==(Rect rect) const;
    };
}
