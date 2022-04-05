#pragma once
#include <objbase.h>
#include<exception>
#include<stdexcept>

class Assert
{
public:
	static void IsSucceeded(LRESULT result, const char* errorLog = "错误断言：函数的执行结果实际是失败的。");
};