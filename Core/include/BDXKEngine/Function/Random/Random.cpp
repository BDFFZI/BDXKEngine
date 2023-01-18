#include "Random.h"
#include <random>
#include <ctime>

namespace BDXKEngine
{
    unsigned int Random::seed = static_cast<unsigned int>(time(nullptr));

    float Random::FloatValue(float min, float max)
    {
        seed++;
        std::default_random_engine engine(seed);
        std::uniform_real_distribution<> random(static_cast<double>(min), static_cast<double>(max));
        return static_cast<float>(random(engine));
    }
    int Random::IntValue(int min, int max)
    {
        seed++;
        std::default_random_engine engine(seed);
        std::uniform_int_distribution<> random(min, max);
        return random(engine);
    }
    Color Random::ColorHSV()
    {
        return Color{
            FloatValue(),
            FloatValue(),
            FloatValue(),
            FloatValue(),
        };
    }
    Vector3 Random::SphereInside()
    {
        const Vector3 suface = SphereSurface();
        return Vector3::Lerp(-suface, suface, FloatValue());
    }
    Vector3 Random::SphereSurface()
    {
        return Vector3{
            FloatValue(),
            FloatValue(),
            FloatValue(),
        }.GetNormalized();
    }
}
