#include "Transferer.h"
#include <regex>

namespace BDXKEngine
{
    bool Transferer::HasTransfer(const Type& type) const
    {
        return transferFuncs.contains(type);
    }

    void Transferer::PushPath(const std::string& key)
    {
    }
    void Transferer::PopPath(std::string& key)
    {
    }

    void Transferer::TransferFieldNative(std::string key, void* value, const Type& type)
    {
        PushPath(key);
        TransferValueNative(value, type);
        PopPath(key);
    }
    void Transferer::TransferValueNative(void* value, const Type& type)
    {
        const auto transferFunc = GetTransferFunc(type);
        if (transferFunc != nullptr)
            transferFunc(value, type);
        else
        {
            if (type.find("class std::vector") != std::string::npos) //是向量类型
            {
                //解析向量的模板类型
                std::smatch matchResult;
                std::regex_search(type, matchResult, std::regex{"class std::vector<(.*)>"}); //(?<=<).*(?=,)
                const std::string matchString = matchResult[1];
                Type itemType;
                size_t itemSize;
                if (std::regex_search( //是vector嵌套
                        matchString, matchResult,
                        std::regex{"(class std::vector<.*,class std::allocator<.*> >),class std::allocator"}) == true
                )
                {
                    itemType = matchResult[1];
                    itemSize = sizeof std::vector<void*>;
                }
                else //是基本元素
                {
                    size_t angleBracketsIndex = matchString.find('<');
                    const size_t commaIndex = matchString.find(',');
                    if (commaIndex > angleBracketsIndex)
                    {
                        //是泛型
                        int stack = 1;
                        for (angleBracketsIndex += 1; angleBracketsIndex < matchString.length(); angleBracketsIndex++)
                        {
                            if (matchString[angleBracketsIndex] == '<')
                                stack++;
                            else if (matchString[angleBracketsIndex] == '>')
                                stack--;

                            if (stack == 0)
                                break;
                        }

                        itemType = matchString.substr(0, angleBracketsIndex + 1);
                        itemSize = GetTransferSize(itemType);
                    }
                    else
                    {
                        //常规类型
                        itemType = matchString.substr(0, commaIndex);
                        itemSize = GetTransferSize(itemType);
                    }
                }

                if (itemSize != 0)
                {
                    if (itemType == GetTypeOf<bool>()) //虽然bool以byte为单位，但vector<bool>似乎以bit的方式储存，所以得特殊处理
                    {
                        std::vector<bool>& vector = *static_cast<std::vector<bool>*>(value);
                        int count = static_cast<int>(vector.size());

                        TransferField("count", count);
                        vector.resize(count);
                        for (int index = 0; index < count; index++)
                        {
                            bool element = vector[index];
                            TransferField("item" + std::to_string(index), element);
                            vector[index] = element;
                        }
                    }
                    else if (itemType == GetTypeOf<std::string>()) //有虚表，特殊处理
                    {
                        std::vector<std::string>& vector = *static_cast<std::vector<std::string>*>(value);

                        int count = static_cast<int>(vector.size());
                        TransferField("count", count);
                        vector.resize(count);
                        for (int index = 0; index < count; index++)
                        {
                            TransferField("item" + std::to_string(index), vector[index]);
                        }
                    }
                    else //注意以这种方式创建的对象没有虚表信息，可能会出问题，需自行额外处理
                    {
                        std::vector<char>& vector = *static_cast<std::vector<char>*>(value);
                        int count = static_cast<int>(vector.size() / itemSize);

                        TransferField("count", count);
                        vector.resize(count * itemSize);
                        for (int index = 0; index < count; index++)
                        {
                            TransferFieldNative("item" + std::to_string(index), vector.data() + index * itemSize, itemType);
                        }
                    }

                    return;
                }
            }

            throw std::exception((type + "无法找到可用的传输方式").c_str());
        }
    }
    std::function<void(void*, const Type&)> Transferer::GetTransferFunc(const Type& type)
    {
        const auto transferFunc = transferFuncs.find(type);
        if (transferFunc != transferFuncs.end())
            return transferFunc->second;
        return nullptr;
    }
    int Transferer::GetTransferSize(const Type& type)
    {
        const auto transferSize = transferSizes.find(type);
        if (transferSize != transferSizes.end())
            return transferSize->second;
        return 0;
    }
}
