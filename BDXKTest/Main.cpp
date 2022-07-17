#include "Test.h"
using namespace BDXKEngine;
using namespace BDXKEditor;

int main()
{
	BDXKEngine::Engine::Run(
		[&]() {
			CreationMenu::Camera(L"摄像机");
			CreationMenu::Light::DirectionalLight(L"太阳");
			CreationMenu::Object3D::Plane(L"地面");
			//ObjectPtr<GameObject> sphere = CreationMenu::Object3D::Sphere(L"球");
		}
	);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	return main();
}