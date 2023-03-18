#include "Rect.h"
#include "../Mathematics/Math.h"

namespace BDXKEngine
{
    const Rect Rect::zero = {};

    Rect::Rect()
    {
        x = 0;
        y = 0;
        width = 0;
        height = 0;
    }
    Rect::Rect(float x, float y, float width, float height)
    {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }
    Rect::Rect(Vector2 min, Vector2 max)
    {
        x = min.x;
        y = min.y;
        width = max.x - min.x;
        height = max.y - min.y;
    }

    Vector2 Rect::GetPosition() { return {x, y}; }
    Vector2 Rect::GetSize() { return {width, height}; }
    Vector2 Rect::GetMin() { return {x, y}; }
    Vector2 Rect::GetMax() const { return {x + width, y + height}; }
    float Rect::GetXMin() const { return x; }
    float Rect::GetYMin() const { return y; }
    float Rect::GetXMax() const { return x + width; }
    float Rect::GetYMax() const { return y + height; }
    void Rect::SetSize(Vector2 size)
    {
        width = size.x;
        height = size.y;
    }
    void Rect::SetPosition(Vector2 position)
    {
        x = position.x;
        y = position.y;
    }

    bool Rect::Contains(Vector2 position) const
    {
        if (position.x < x || position.x > GetXMax() || position.y < y || position.y > GetYMax())
            return false;
        return true;
    }

    bool Rect::operator==(Rect rect) const
    {
        return Equal(x, rect.x) && Equal(y, rect.y) && Equal(width, rect.width) && Equal(height, rect.height);
    }
}
