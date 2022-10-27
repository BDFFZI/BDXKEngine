#include <BDXKEngine/Engine.h>
#include <BDXKEditor/BDXKEditor.h>
#include "Test.h"
using namespace BDXKEngine;
using namespace BDXKEditor;

int main()
{
    RunEditor();
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    return main();
}
