#include <BDXKEngine.h>
#include <BDXKEditor.h>
#include "Test.h"
using namespace BDXKEngine;
using namespace BDXKEditor;

int main()
{
    Editor::Run();
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    return main();
}
