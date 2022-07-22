#pragma once
#include<Windows.h>
#include<iostream>
#include"String.h"

namespace BDXKEngine {
	class Debug
	{
	public:
		static void Log(String str);

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
		static void Log(std::wstring str, int color);

		static void LogWarning(std::wstring str);

		static void LogError(std::wstring str);
	};
}

