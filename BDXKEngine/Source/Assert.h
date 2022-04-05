#pragma once
#include <objbase.h>
#include<exception>
#include<stdexcept>

class Assert
{
public:
	static void IsSucceeded(LRESULT result, const char* errorLog = "������ԣ�������ִ�н��ʵ����ʧ�ܵġ�")
	{
		if (FAILED(result))throw std::domain_error(errorLog);
	}
};