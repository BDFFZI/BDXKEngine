#include "String.h"

namespace BDXKEngine {
	std::wstring String::to_wstring(const std::string& input)
	{
		using namespace std;
		const char* source = input.c_str();
		size_t charNum = input.size() + 1;

		wchar_t* dest = new wchar_t[charNum];
		mbstowcs_s(NULL, dest, charNum, source, _TRUNCATE);
		wstring result = dest;
		delete[] dest;
		return result;
	}

	std::string String::to_string(const std::wstring& input)
	{
		using namespace std;
		const wchar_t* wcs = input.c_str();
		size_t dByteNum = sizeof(wchar_t) * input.size() + 1;

		char* dest = new char[dByteNum];
		wcstombs_s(NULL, dest, dByteNum, wcs, _TRUNCATE);
		string result = dest;
		delete[] dest;
		return result;
	}
	
	String::String(wchar_t* value) : std::wstring(value) {}
	String::String(const wchar_t* value) : std::wstring(value) {}
	String::String(std::wstring value) : std::wstring(value) {}
	String::String(char* value) : std::wstring(to_wstring(std::string(value))) {}
	String::String(const char* value) : std::wstring(to_wstring(std::string(value))) {}
	String::String(std::string value) : std::wstring(to_wstring(value)) {}

	String::String(bool value) : std::wstring(value == 1 ? L"True" : L"False") {}
	String::String(std::exception& value) : String(value.what()) {}
}