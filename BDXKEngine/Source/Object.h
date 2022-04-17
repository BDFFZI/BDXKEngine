#pragma once
#include"String.h"

class Object
{
public:
	int id = -1;
	String name = L"";

	virtual String ToString();

	operator String();
};

