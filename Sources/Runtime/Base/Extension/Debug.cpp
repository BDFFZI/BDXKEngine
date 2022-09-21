﻿#include "Debug.h"

namespace BDXKEngine {
	void Debug::Log(String str)
	{
		using namespace std;
		wcout << str << endl;
	}

	/// <summary>
	/// 输出带颜色的日志
	/// 黑色     = 0  | 
	/// 蓝色     = 1  | 
	///	绿色     = 2  | 
	///	浅绿色   = 3  | 
	///	红色     = 4  | 
	///	紫色     = 5  | 
	///	黄色     = 6  | 
	///	白色     = 7  | 
	/// 灰色     = 8  |
	/// 淡蓝色   = 9  |
	/// 淡绿色   = 10 |
	/// 淡浅绿色 = 11 |
	/// 淡红色   = 12 |
	/// 淡紫色   = 13 |
	/// 淡黄色   = 14 |
	/// 亮白色   = 15 |
	/// </summary>
	void Debug::Log(std::wstring str, int color)
	{
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | color);
		Log(str);
		SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | 7);
	}

	void Debug::LogWarning(std::wstring str)
	{
		Log(str, 6);
	}

	void Debug::LogError(std::wstring str)
	{
		Log(str, 4);
	}
}
