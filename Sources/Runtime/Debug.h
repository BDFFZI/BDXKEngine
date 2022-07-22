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
		/// �������ɫ����־
		/// ��ɫ     = 0  | 
		/// ��ɫ     = 1  | 
		///	��ɫ     = 2  | 
		///	ǳ��ɫ   = 3  | 
		///	��ɫ     = 4  | 
		///	��ɫ     = 5  | 
		///	��ɫ     = 6  | 
		///	��ɫ     = 7  | 
		/// ��ɫ     = 8  |
		/// ����ɫ   = 9  |
		/// ����ɫ   = 10 |
		/// ��ǳ��ɫ = 11 |
		/// ����ɫ   = 12 |
		/// ����ɫ   = 13 |
		/// ����ɫ   = 14 |
		/// ����ɫ   = 15 |
		/// </summary>
		static void Log(std::wstring str, int color);

		static void LogWarning(std::wstring str);

		static void LogError(std::wstring str);
	};
}

