#include "Test.h"

int main()
{
	BDXKEngine::Engine::Run(
		[&]() {
			Assembly::Run2();
		}
	);

	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	return main();
}