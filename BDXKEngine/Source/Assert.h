#pragma once
#include <objbase.h>
#include<exception>
#include<stdexcept>

class Assert
{
public:
	static void IsSucceeded(LRESULT result, const char* errorLog = "������ԣ�������ִ�н��ʵ����ʧ�ܵġ�");
};