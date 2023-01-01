#include "Random.h"
#include <random>
#include <ctime>

namespace BDXKEngine
{
    unsigned int Random::seed = static_cast<unsigned int>(time(nullptr));

    Color Random::ColorHSV()
    {
        seed++;

        std::default_random_engine engine(seed);
        std::uniform_int_distribution<> random(0, 255);
        return Color{
            static_cast<float>(random(engine)) / 255.0f,
            static_cast<float>(random(engine)) / 255.0f,
            static_cast<float>(random(engine)) / 255.0f,
            static_cast<float>(random(engine)) / 255.0f
        };
    }
    Vector3 Random::InsideUnitSphere()
    {
        return {};
    }
    Vector3 Random::OnUnitSphere()
    {
        return {};
    }
}
