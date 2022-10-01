#include "String.h"

#include <rapidjson/encodings.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/stream.h>

namespace BDXKEngine
{
    char* String::to_char(const char8_t* value)
    {
        return reinterpret_cast<char*>(const_cast<char8_t*>(value));
    }

    std::wstring String::to_wstring(const std::string& input)
    {
        using namespace std;
        const char* source = input.c_str();
        const size_t charNum = input.size() + 1;
        
        wchar_t* dest = new wchar_t[charNum];
        mbstowcs_s(NULL, dest, charNum, source, _TRUNCATE);
        wstring result = dest;
        delete[] dest;
        return result;
    }

    std::string String::to_string(const std::wstring& input)
    {
        using namespace std;
        const wchar_t* wcs = input.c_str();
        size_t dByteNum = sizeof(wchar_t) * input.size() + 1;

        char* dest = new char[dByteNum];
        wcstombs_s(NULL, dest, dByteNum, wcs, _TRUNCATE);
        string result = dest;
        delete[] dest;
        return result;
    }
}
