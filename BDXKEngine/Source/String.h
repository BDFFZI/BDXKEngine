#pragma once
#include<string>
#include<sstream>

///模仿C# 自动ToString()和operator+
class String :public std::wstring
{
public:
	static std::wstring to_wstring(const std::string& input);

	static std::string to_string(const std::wstring& input);

	String() {}

	//原生字符串
	String(wchar_t* value);
	String(const wchar_t* value);
	String(std::wstring value);
	String(char* value);
	String(const char* value);
	String(std::string value);
	//布尔类型
	String(bool value);
	//异常
	String(std::exception& value);

	//默认数据类型转换
	template<typename TDataType>
	String(TDataType value) : std::wstring(std::to_wstring(value)) {}
	//带格式转换
	template<typename TDataType>
	String(TDataType value, std::ios_base& (*format)(std::ios_base&)) {
		std::wstringstream stringStream;
		stringStream << format << value;
		append(stringStream.str());
	}
	//指针类型
	template<typename TDataType>
	String(TDataType* value) : std::wstring(to_wstring(typeid(value).name())) {
		if (value == NULL)append(L"[Null]");
	}
	//连加自动转换
	template<typename TDataType>
	String operator+(const TDataType value)
	{
		String str = *this;
		str.append((String)value);
		return str;
	}
};


