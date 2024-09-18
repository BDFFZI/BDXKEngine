#pragma once
#include <string>

namespace BDXKEngine
{
    class Debug
    {
    public:
        /**
           * \brief 普通文本
           */
        static void Log(const std::string& log);
        /**
           * \brief 高亮为警告
           */
        static void LogWarning(const std::string& warning);
        /**
           * \brief 高亮为错误
           */
        static void LogError(const std::string& error);
        /**
   * \brief 高亮为错误并带有错误代码，一般第三方库需要用这个回调错误
   */
        static void LogError(int error_code, const char* description);
        /**
           * \brief 高亮为错误的同时带有堆栈信息
           */
        static void LogException(const std::exception& exception);
    };
}
