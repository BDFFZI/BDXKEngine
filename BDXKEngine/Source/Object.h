#pragma once
#include"String.h"

class Object
{
public:
	int id;
	String name;

	virtual String ToString();

	operator String();
};

