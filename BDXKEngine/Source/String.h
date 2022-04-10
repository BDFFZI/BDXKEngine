#pragma once
#include<iostream>
#include<string>
#include <codecvt>
#include<sstream>

///ģ��C# �Զ�ToString()��operator+
class String :public std::wstring
{
public:
	static std::wstring to_wstring(const std::string& input);

	static std::string to_string(const std::wstring& input);

	String() {};
	//Ĭ����������ת��
	template<typename TDataType>
	String(TDataType value) : std::wstring(std::to_wstring(value)) {}
	//�����Զ�ת��
	template<typename TDataType>
	String operator+(const TDataType value)
	{
		String str = *this;
		str.append((String)value);
		return str;
	}

	//����ת������

	//ԭ���ַ���
	String(const wchar_t* value) :std::wstring(value) {}
	String(wchar_t* value) :std::wstring(value) {}
	String(const char* value) :std::wstring(to_wstring(std::string(value))) {}
	String(char* value) :std::wstring(to_wstring(std::string(value))) {}
	//����ʽת��
	template<typename TDataType>
	String(TDataType value,  std::ios_base& (*format)(std::ios_base&)) {
		std::wstringstream stringStream;
		stringStream << format << value;
		append(stringStream.str());
	}
	//��������
	String(bool value) :std::wstring(value == 1 ? L"True" : L"False") {}
	///ָ������
	template<typename TDataType>
	String(TDataType* value) : std::wstring(to_wstring(typeid(value).name())) {
		if (value == NULL)append(L"[Null]");
	}
	//�쳣
	String(std::exception& value) :String(value.what()) {}
};


