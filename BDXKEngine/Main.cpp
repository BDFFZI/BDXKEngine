#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <iostream>
using namespace std;
#include "BDXKEngine.h"

int main()
{
	MainWindow window;
	window.Show();

	//检索消息队列
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		//预处理后交给窗口过程响应
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}