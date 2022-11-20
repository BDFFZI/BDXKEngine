#pragma once
#include<string>
#include<sstream>
#include <codecvt>

namespace BDXKEngine
{
    ///模仿C# 自动ToString()和operator+
    ///请当成辅助工具来使用
    class String
    {
    public:
        static std::wstring to_wstring(const std::string& input);

        static std::string to_string(const std::wstring& input);
    };
}
