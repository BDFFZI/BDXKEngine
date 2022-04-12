#pragma once
//C++匈牙利表示法，允许组合使用
//W 宽，如wchar_t,以处理Unicode中utf-16,注意现在都用这个
//L 长，x16时代的产物，现在不带也是长
//P 指针
//LP 长指针
//C 常量

//C++特殊函数后缀，代表函数的不同版本
// 
//函数中使用的字符类型
//A ASCII编码
//W 宽字符编码，咱中文就用这个
// 
//一般表明颜色存储方式
//U	无符号整数 如：0-255
//F 浮点数 如：0.0-1.0

//C++数据类型
//PCWSTR = 常量宽字符指针,标准字符串
//LRESULT = long int，表示函数的返回代码
//HWND = 窗口句柄
//BOOL = int
//WPARAM = uint
//LPARAM = int

//基础数据结构
#include "String.h"
#include "Com.h"
#include "Color.h"
#include "Vector2.h"
#include "Math.h"
#include "Time.h"
//扩展功能
#include "WindowBase.h"
#include "DialogWindow.h"
#include "MainWindow.h"
#include "Debug.h"
#include "Assert.h"
#include "GameObject.h"
#include "Component.h"

class BDXKEngine {
public:
	static void Awake();

	static void Update();

	static void Run();

private:
	static MainWindow *window;
};