#include "Assert.h"
#include "String.h"
#include "Debug.h"

void Assert::IsSucceeded(LRESULT result, const char* errorLog)
{
	if (FAILED(result)) {
		Debug::LogError(String(result, std::hex));
		throw std::domain_error(errorLog);
	}
}
