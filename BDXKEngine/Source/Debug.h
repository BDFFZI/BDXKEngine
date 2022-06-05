#pragma once
#include<Windows.h>
#include<iostream>

struct Debug
{
public:
	static void Log(const wchar_t* str);

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
	static void Log(const wchar_t* str, int color);

	static void LogWarning(const wchar_t* str);

	static void LogError(const wchar_t* str);
};


