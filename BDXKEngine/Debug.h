#pragma once
#include<iostream>
using namespace std;
static class Debug
{
public:
	static void Log(HWND pointer)
	{
		Log(pointer != NULL);
	}

	static void Log(bool value)
	{
		if (value == true)
			cout << "True" << endl;
		else
			cout << "False" << endl;
	}

	static void Log()
	{

	}
};

