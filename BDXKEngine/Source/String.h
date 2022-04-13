#pragma once
#include<iostream>
#include<string>
#include <codecvt>
#include<sstream>

///模仿C# 自动ToString()和operator+
class String :public std::wstring
{
public:
	static std::wstring to_wstring(const std::string& input);

	static std::string to_string(const std::wstring& input);

	String() {};
	//默认数据类型转换
	template<typename TDataType>
	String(TDataType value) : std::wstring(std::to_wstring(value)) {}
	//连加自动转换
	template<typename TDataType>
	String operator+(const TDataType value)
	{
		String str = *this;
		str.append((String)value);
		return str;
	}

	//数据转换补充

	//原生字符串
	String(const wchar_t* value) :std::wstring(value) {}
	String(wchar_t* value) :std::wstring(value) {}
	String(const char* value) :std::wstring(to_wstring(std::string(value))) {}
	String(char* value) :std::wstring(to_wstring(std::string(value))) {}
	String(std::wstring value) :std::wstring(value) {}
	String(std::string value) :std::wstring(to_wstring(value)) {}
	//带格式转换
	template<typename TDataType>
	String(TDataType value, std::ios_base& (*format)(std::ios_base&)) {
		std::wstringstream stringStream;
		stringStream << format << value;
		append(stringStream.str());
	}
	//布尔类型
	String(bool value) :std::wstring(value == 1 ? L"True" : L"False") {}
	///指针类型
	template<typename TDataType>
	String(TDataType* value) : std::wstring(to_wstring(typeid(value).name())) {
		if (value == NULL)append(L"[Null]");
	}
	//异常
	String(std::exception& value) :String(value.what()) {}
};


