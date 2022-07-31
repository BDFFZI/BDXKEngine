#include "Test.h"
using namespace BDXKEngine;
using namespace BDXKEditor;

int main()
{
	BDXKEngine::Engine::Run(
		[&]() {
			//Assembly::CreateDefaultScene();
			Assembly::TestLight();
			//ObjectPtr<Camera> camera = GameObject::Find(L"摄像机")->GetComponent<Camera>();
			//camera->GetGameObject()->AddComponent<Assembly::CreateCube>();
		}
	);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	return main();
}