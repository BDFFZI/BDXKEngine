#include "ObjectBase.h"

String ObjectBase::ToString()
{
	return typeid(*this).name();
}

ObjectBase::operator String()
{
	return ToString();
}