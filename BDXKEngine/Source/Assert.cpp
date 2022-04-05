#include "Assert.h"

void Assert::IsSucceeded(LRESULT result, const char* errorLog)
{
	if (FAILED(result))throw std::domain_error(errorLog);
}
