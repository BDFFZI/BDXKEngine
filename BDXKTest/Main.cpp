#include "Test.h"
using namespace BDXKEngine;
using namespace BDXKEditor;

#include<fstream>
#include<rapidjson\document.h>
#include<rapidjson\writer.h>
#include<rapidjson\stringbuffer.h>

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

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	return main();
}