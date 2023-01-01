#include <iostream>

#include "BDXKEngine/Base/Data/Mathematics/Vector/Vector3.h"

using namespace BDXKEngine;

int main(int argc, char* argv[])
{
    Vector3 vector3 = {1,2,3};
    std::cout << vector3.ToString();
    return 0;
}
