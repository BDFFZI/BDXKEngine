#include "Assert.h"

void Assert::IsSucceeded(LRESULT result, const wchar_t* errorLog)
{
	if (FAILED(result)) {
		std::wstringstream string;
		string << "[0x";
		string << std::uppercase << std::hex << (int)result;
		string << "] " << errorLog;
		Debug::LogError(string.str().c_str());
		throw - 1;
	}
}

void Assert::IsTrue(bool result, const wchar_t* errorLog)
{
	if (result == false) {
		Debug::LogError(errorLog);
		throw - 1;
	}
}
