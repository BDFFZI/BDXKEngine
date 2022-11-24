#pragma once
#include <string>
#include <any>

namespace BDXKEngine
{
    typedef std::string Type;

    template <typename T>
    Type GetTypeIDOf()
    {
        return typeid(T).name();
    }
    template <typename T>
    Type GetTypeID(const T& value)
    {
        return typeid(value).name();
    }
}
