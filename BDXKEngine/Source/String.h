#pragma once
#include<string>
#include<sstream>

///ģ��C# �Զ�ToString()��operator+
class String :public std::wstring
{
public:
	static std::wstring to_wstring(const std::string& input);

	static std::string to_string(const std::wstring& input);

	String() {}

	//ԭ���ַ���
	String(wchar_t* value);
	String(const wchar_t* value);
	String(std::wstring value);
	String(char* value);
	String(const char* value);
	String(std::string value);
	//��������
	String(bool value);
	//�쳣
	String(std::exception& value);

	//Ĭ����������ת��
	template<typename TDataType>
	String(TDataType value) : std::wstring(std::to_wstring(value)) {}
	//����ʽת��
	template<typename TDataType>
	String(TDataType value, std::ios_base& (*format)(std::ios_base&)) {
		std::wstringstream stringStream;
		stringStream << format << value;
		append(stringStream.str());
	}
	//ָ������
	template<typename TDataType>
	String(TDataType* value) : std::wstring(to_wstring(typeid(value).name())) {
		if (value == NULL)append(L"[Null]");
	}
	//�����Զ�ת��
	template<typename TDataType>
	String operator+(const TDataType value)
	{
		String str = *this;
		str.append((String)value);
		return str;
	}
};


