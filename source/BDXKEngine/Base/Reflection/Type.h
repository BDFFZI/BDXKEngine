#pragma once
#include <string>
#include <any>

namespace BDXKEngine
{
    typedef std::string Type;

    template <typename T>
    Type GetTypeOf()
    {
        return typeid(T).name();
    }
    template <typename T>
    Type GetType(const T& value)
    {
        return typeid(value).name();
    }
}
