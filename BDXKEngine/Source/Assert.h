#pragma once
#include <objbase.h>
#include<sstream>
#include "Debug.h"

struct Assert
{
public:
	static void IsSucceeded(LRESULT result, const wchar_t* errorLog = L"������ԣ�������ִ�н��ʵ����ʧ�ܵġ�");
	static void IsTrue(bool result, const wchar_t* errorLog = L"������ԣ������ʵ���Ƿ񶨵ġ�");
};