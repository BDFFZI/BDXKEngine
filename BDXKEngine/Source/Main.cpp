#include "BDXKEngine.h"
#include "Test.h"
#include<iostream>
#include"GL.h"

int main()
{
	std::setlocale(LC_ALL, "zh-CN");

	WindowBase window(L"Direct3D Test");
	window.Show();

	GL::CreateDevice();
	GL::CreateSwapChain(window.GetHwnd());
	GL::CreateRenderTexture();

	Mesh mesh = {};


	while (true)
	{
		mesh.Render();
	}

	//检索消息队列
	//MSG msg = {};
	//while (GetMessage(&msg, NULL, 0, 0))
	//{
	//	//预处理后交给窗口过程响应
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);
	//}

	//Object* object;
	//List<Object*> list;
	//list.push_back(NULL);
	//list.push_back(NULL);
	//list.push_back(NULL);
	//list.push_back(NULL);

	//auto result = std::find_if(list.begin(), list.end(), [](Object* o) {return false; });
	//Debug::Log(result == list.end());
}