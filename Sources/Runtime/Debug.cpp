#include "Debug.h"

namespace BDXKEngine {
	void Debug::Log(std::wstring str)
	{
		using namespace std;
		wcout << str << endl;
	}

	void Debug::Log(std::string str)
	{
		using namespace std;
		cout << str << endl;
	}

	void Debug::Log(std::exception str)
	{
		Log((String)str.what());
	}

	/// <summary>
	/// �������ɫ����־
	/// ��ɫ     = 0  | 
	/// ��ɫ     = 1  | 
	///	��ɫ     = 2  | 
	///	ǳ��ɫ   = 3  | 
	///	��ɫ     = 4  | 
	///	��ɫ     = 5  | 
	///	��ɫ     = 6  | 
	///	��ɫ     = 7  | 
	/// ��ɫ     = 8  |
	/// ����ɫ   = 9  |
	/// ����ɫ   = 10 |
	/// ��ǳ��ɫ = 11 |
	/// ����ɫ   = 12 |
	/// ����ɫ   = 13 |
	/// ����ɫ   = 14 |
	/// ����ɫ   = 15 |
	/// </summary>
	void Debug::Log(std::wstring str, int color)
	{
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

		SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | color);
		Log(str);
		SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | 7);
	}

	void Debug::LogWarning(std::wstring str)
	{
		Log(str, 6);
	}

	void Debug::LogError(std::wstring str)
	{
		Log(str, 4);
	}
}
