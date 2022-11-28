#include "Reflection.h"

#include <ranges>

namespace BDXKEngine
{
    ReflectionTransferer::ReflectionTransferer(const Reflective& reflective, Reflection& reflection):
        reflection(reflection)
    {
        origin = reinterpret_cast<std::uintptr_t>(&reflective);
        length = reflection.size;
    }

    void ReflectionTransferer::PushPath(const std::string& key)
    {
        currentKey = key;
    }
    void ReflectionTransferer::TransferValue(void* value, const Type& type)
    {
        const int addressOffset = static_cast<int>(reinterpret_cast<std::uintptr_t>(value) - origin);
        if (addressOffset >= 0 && addressOffset < length)
        {
            reflection.fieldName.emplace_back(currentKey);
            reflection.fieldPos.emplace_back(addressOffset);
            reflection.fieldType.emplace_back(type);
        }
    }

    const Reflection& Reflection::GetReflection(const Type& id)
    {
        const auto& item = reflections.find(id);
        if (item == reflections.end())throw std::exception((id + "的反射信息未定义").c_str());

        return *reflections[id];
    }
    const Reflection& Reflection::GetReflection(const Reflective* reflective)
    {
        return GetReflection(reflective->GetType());
    }

    Reflection::Reflection(const std::function<Reflective*()>& constructor, int size)
    {
        Reflective* reflective = constructor();

        //获取类型大小
        this->size = size;
        //获取构造函数
        this->constructor = constructor;
        //获取虚表地址
        this->virtualTable = size >= static_cast<int>(sizeof(std::uintptr_t)) ? *reinterpret_cast<std::uintptr_t*>(reflective) : 0;
        //获取字段信息
        ReflectionTransferer reflecttransferer = {*reflective, *this};
        reflective->Transfer(reflecttransferer);
        //注册反射
        reflections[reflective->GetType()] = this;

        delete reflective;
    }

    int Reflection::GetSize() const
    {
        return size;
    }
    Reflective* Reflection::GetConstruction() const
    {
        return constructor();
    }
    std::uintptr_t Reflection::GetVirtualTable() const
    {
        return virtualTable;
    }
    void* Reflection::GetField(Reflective* target, const std::string& key) const
    {
        const auto& item = std::ranges::find(fieldName, key);
        if (item == fieldName.end()) throw std::exception((key + "的字段信息未定义").c_str());

        return reinterpret_cast<char*>(target) + fieldPos[(item - fieldName.begin())];
    }
    int Reflection::GetFields(Reflective* target, std::vector<std::string>& names, std::vector<void*>& values,
                              std::vector<Type>& types) const
    {
        names.clear();
        values.clear();
        types.clear();
        const int count = static_cast<int>(fieldName.size());
        for (int i = 0; i < count; i++)
        {
            names.emplace_back(fieldName.at(i));
            values.emplace_back(reinterpret_cast<char*>(target) + fieldPos.at(i));
            types.emplace_back(fieldType.at(i));
        }

        return count;
    }
}
