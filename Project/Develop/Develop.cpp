#include <iostream>
#include <vector>
#include "BDXKEngine/Base/Package/Map.h"
#include "BDXKEngine/Base/Reflection/Reflection.h"
#include "BDXKEngine/Base/Serializer/Core/Serializer.h"
#include "BDXKEngine/Platform/Serialization/SerializerExtension/Json/JsonExporter.h"
#include "BDXKEngine/Platform/Serialization/SerializerExtension/Json/JsonImporter.h"


class X
{
public:
    X(int a)
    {
        b = a;
    }
    ~X()
    {
        printf("");
    }

    int b = 0;
};

X str0 = {123};
X& str = str0;
void A(const X& a)
{
    str = a;
}

int main(int argc, char* argv[])
{
    {
        X a = {321};
        A(a);
    }

    X result = str;
    return 0;
}
