#pragma once
#include <string>

namespace BDXKEngine
{
    typedef std::string Type;

    template <typename T>
    Type GetTypeOf()
    {
        return typeid(T).name();
    }
    template <typename T>
    // ReSharper disable once CppEntityUsedOnlyInUnevaluatedContext
    Type GetType(const T& value)
    {
        return typeid(value).name();
    }

    std::string GetTypeName(const Type& type);
}
