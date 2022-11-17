#include "Transferer.h"
#include <regex>

namespace BDXKEngine
{
    void Transferer::TransferNested(std::string key, Transferable& value)
    {
        PushPath(key);
        value.Transfer(*this);
        PopPath(key);
    }

    void Transferer::Reset(std::string& data)
    {
    }
    void Transferer::PushPath(const std::string& key)
    {
    }
    void Transferer::PopPath(std::string& key)
    {
    }

    void Transferer::TransferValue(void* value, const Type& type)
    {
        const auto transferFunc = transferFuncs[type];
        if (transferFunc != nullptr)transferFunc(value, type);
        else TransferValueFallback(value, type);
    }
    void Transferer::TransferValueFallback(void* value, const Type& type)
    {
        if (type.find("class std::vector") != std::string::npos)
        {
            //解析模板类型
            std::smatch matchResult;
            std::regex_search(type, matchResult, std::regex{"class std::vector<(.*)>"}); //(?<=<).*(?=,)
            const std::string matchString = matchResult[1];

            Type itemType;
            size_t itemSize;
            if (std::regex_search(
                    matchString, matchResult,
                    std::regex{"(class std::vector<.*,class std::allocator<.*> >),class std::allocator"}) == true
            )
            {
                itemType = matchResult[1];
                itemSize = sizeof std::vector<void*>;
            }
            else
            {
                std::regex_search(matchString, matchResult, std::regex{"(.*?),"});

                itemType = matchResult[1];
                itemSize = typeSizes[itemType];
            }


            if (itemSize != 0)
            {
                std::vector<char>& vector = *static_cast<std::vector<char>*>(value);
                int count = static_cast<int>(vector.size() / itemSize);

                TransferValue(count);
                vector.resize(count * itemSize);
                for (int index = 0; index < count; index++)
                    TransferValue(vector.data() + index * itemSize, itemType);
                return;
            }
        }

        throw std::exception((type + "的传输方式未定义").c_str());
    }
}
