#pragma once
#include <vector>

namespace BDXKEngine
{
    class Vector
    {
    public:
        template <typename TValue>
        static void Remove(std::vector<TValue>& vector, const TValue value)
        {
            vector.erase(std::find(
                vector.begin(),
                vector.end(),
                value
            ));
        }
    };
}
