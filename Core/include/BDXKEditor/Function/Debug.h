﻿#pragma once
#include <string>

namespace BDXKEditor
{
    class Debug
    {
    public:
        static void Log(const std::string& str);
        static void Log(const char* str);
        template <typename TData>
        static void Log(const TData& str)
        {
            Log(std::to_string(str));
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
        static void Log(const std::string& str, unsigned short color);

        static void LogWarning(const std::string& str);

        static void LogError(const std::string& str);

        [[noreturn]]
        static void LogException(const std::string& str);
    };
}
