#pragma once
#include "Debug.h"

class Exception
{
public:
	Exception(String type, String message)
	{
		std::wstringstream stream;
		stream << "[";
		stream << type;
		stream << "] ";
		stream << message;
		stream << std::endl;
		Debug::LogError(stream.str());
		throw -1;
	};
};

class MisuseException :public Exception
{
public:
	MisuseException(String message) :Exception(L"Îó²Ù×÷Òì³£", message) {}
};