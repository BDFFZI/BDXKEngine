#pragma once
#include<iostream>
#include<string>

///ģ��C# �Զ�ToString()��operator+
class String :public std::string
{
public:
	template<typename TDataType>String(TDataType value) : std::string(std::to_string(value)) {}
	template<typename TDataType>
	String operator+(const TDataType value)
	{
		String str = *this;
		str.append((String)value);
		return str;
	}

	//��ʽת������

	String(const char* value) :std::string(value) {}
	String(bool value) :std::string(value == 1 ? "True" : "False") {}
	///ָ������
	template<typename TDataType>
	String(TDataType* value) : std::string(typeid(value).name()) {
		if (value == NULL)append("[Null]");
	}
};


