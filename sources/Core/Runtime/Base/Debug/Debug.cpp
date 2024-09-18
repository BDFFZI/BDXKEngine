#include "Debug.h"
#include <iostream>
#include <regex>

namespace BDXKEngine
{
    void Debug::Log(const std::string& log)
    {
        std::cout << log << std::endl;
    }
    void Debug::LogWarning(const std::string& warning)
    {
        std::cout << "[!]" << warning << std::endl;
    }
    void Debug::LogError(const std::string& error)
    {
        std::cout << "[X]" << error << std::endl;
    }
    void Debug::LogError(const int error_code, const char* description)
    {
        std::cout << "[X]" << error_code << ':' << description << std::endl;
    }
    void Debug::LogException(const std::exception& exception)
    {
        std::cout << "[X]" << exception.what() << std::endl;
    }
}
