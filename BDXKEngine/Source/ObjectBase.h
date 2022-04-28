#pragma once
#include"String.h"

class ObjectBase
{
	virtual String ToString();

	operator String();
};

