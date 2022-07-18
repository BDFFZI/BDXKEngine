#include "Test.h"
using namespace BDXKEngine;
using namespace BDXKEditor;

int main()
{
	BDXKEngine::Engine::Run(
		[&]() {
			Assembly::TestLight();
			ObjectPtr<GameObject> camera = GameObject::Find(L"摄像机");
			camera->AddComponent<Assembly::CreateCube>();
		}
	);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	return main();
}