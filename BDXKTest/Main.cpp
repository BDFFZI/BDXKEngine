#include "Test.h"
using namespace BDXKEngine;

int main()
{
	BDXKEngine::Engine::Run(
		[&]() {
			Assembly::Run1();
		}
	);

	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	return main();
}