#pragma once
#include <objbase.h>
#include <ShObjIdl.h>
#include<exception>
#include<stdexcept>
#include "String.h"

class DialogUtility
{
public:
	static String FileOpenDialog();
	static String FileSaveDialog();
};

