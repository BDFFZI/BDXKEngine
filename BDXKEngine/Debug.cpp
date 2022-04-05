#include "Debug.h"

void Debug::Log(String str)
{
	using namespace std;
	wcout << str << endl;
}

void Debug::Log(String str, int color)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | color);
	Log(str);
	SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | 7);
}

void Debug::LogWarning(String str)
{
	Log(str, 6);
}

 void Debug::LogError(String str)
{
	Log(str, 4);
}
