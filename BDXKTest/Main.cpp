#include <BDXKEngine.h>
#include "Test/Test.h"
using namespace BDXKEngine;
using namespace BDXKEditor;

int main()
{
    Engine::Run(
        [&]()
        {
            Assembly::CreateDefaultScene();
            Assembly::TestTransparency();
            Assembly::TestLight();
            Assembly::TestShadow();
        }
    );

    std::getchar();
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    return main();
}
