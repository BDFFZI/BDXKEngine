#include "Object.h"
#include <typeinfo>

String Object::ToString()
{
	return typeid(*this).name();
}

Object::operator String()
{
	return ToString();
}
