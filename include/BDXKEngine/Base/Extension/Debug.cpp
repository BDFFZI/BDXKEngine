#include "Debug.h"
#include <windows.h>

namespace BDXKEngine
{
    void Debug::Log(const std::string& str)
    {
        using namespace std;
        cout << str << endl;
    }

    /// <summary>
    /// 输出带颜色的日志
    /// 黑色     = 0  | 
    /// 蓝色     = 1  | 
    ///	绿色     = 2  | 
    ///	浅绿色   = 3  | 
    ///	红色     = 4  | 
    ///	紫色     = 5  | 
    ///	黄色     = 6  | 
    ///	白色     = 7  | 
    /// 灰色     = 8  |
    /// 淡蓝色   = 9  |
    /// 淡绿色   = 10 |
    /// 淡浅绿色 = 11 |
    /// 淡红色   = 12 |
    /// 淡紫色   = 13 |
    /// 淡黄色   = 14 |
    /// 亮白色   = 15 |
    /// </summary>
    void Debug::Log(const std::string& str, unsigned short color)
    {
        const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

        SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | color);
        Log(str);
        SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | 7);
    }

    void Debug::LogWarning(const std::string& str)
    {
        Log(str, 6);
    }

    void Debug::LogError(const std::string& str)
    {
        Log(str, 4);
    }

    void Debug::LogException(const std::string& str)
    {
        LogError(str);
        throw std::exception(str.c_str());
    }
}
