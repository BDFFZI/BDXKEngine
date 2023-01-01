#pragma once
#include<string>
#include<sstream>
#include <codecvt>

namespace BDXKEngine
{
    class String
    {
    public:
        static std::wstring to_wstring(const std::string& input);
        static std::string to_string(const std::wstring& input);
        
        static int EncodingBase64(const unsigned char* in_data, int in_count, char* out_base64);
        static int DecodingBase64(const char* in_base64, int in_charcount, unsigned char* out_data);
    };
}
