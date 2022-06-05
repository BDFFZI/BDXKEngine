#pragma once
#include <objbase.h>
#include<sstream>
#include "Debug.h"

struct Assert
{
public:
	static void IsSucceeded(LRESULT result, const wchar_t* errorLog = L"错误断言：函数的执行结果实际是失败的。");
	static void IsTrue(bool result, const wchar_t* errorLog = L"错误断言：结果事实上是否定的。");
};