#include "BDXKEngine.h"

int main()
{
	Debug::Log(true);
	Debug::LogWarning(23.23f);
	Debug::LogError("文本");
	Debug::Log((String*)0, 1);
	Debug::Log((String)"微软文档永远的神" + false + 123);

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