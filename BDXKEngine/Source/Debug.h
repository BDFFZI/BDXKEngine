#pragma once
#include<Windows.h>
#include<iostream>

struct Debug
{
public:
	static void Log(const wchar_t* str);

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
	static void Log(const wchar_t* str, int color);

	static void LogWarning(const wchar_t* str);

	static void LogError(const wchar_t* str);
};


