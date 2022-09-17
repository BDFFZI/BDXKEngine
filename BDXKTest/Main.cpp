#include "Test.h"
//using namespace BDXKEngine;
//using namespace BDXKEditor;

int main()
{
	BDXKEngine::Engine::Run(
		[&]() {
			Assembly::CreateDefaultScene();
			Assembly::TestTransparency();
			Assembly::TestLight();
			Assembly::TestShadow();
		}
	);
}

//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
//{
//	return main();
//}