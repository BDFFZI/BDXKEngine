// #include <BDXKEngine/Engine.h>
// #include <BDXKEditor/BDXKEditor.h>
// #include "Test.h"
// using namespace BDXKEngine;
// using namespace BDXKEditor;
//
// int main()
// {
//     RunEditor();
// }
//
// int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
// {
//     return main();
// }

#include <iostream>
#include "BDXKEngine/Base/Serialization/Serialization.h"


struct A : BDXKEngine::Serialization
{
};

struct B : A
{
};

int main()
{
    A* a = new B();
    //std::cout << a->GetTypeID();
    getchar();
}
