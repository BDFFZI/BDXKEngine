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
    int Reflection::GetReflections(std::vector<Reflection*>& reflections, const std::function<bool(const Reflection&)> condition)
    {
        reflections.clear();
        for (const auto& item : Reflection::reflections | std::ranges::views::values)
        {
            if (condition(*item) == true)
                reflections.emplace_back(item);
        }

        return static_cast<int>(reflections.size());
    }


    Reflection::Reflection(const std::function<Reflective*()>& constructor, int size)
    {
        //获取类型大小
        this->size = size;
        //获取构造函数
        this->constructor = constructor;
        //获取实例
        instance = constructor();
        //获取字段信息
        ReflectionTransferer reflecttransferer = {*instance, *this};
        instance->Transfer(reflecttransferer);
        //注册反射
        reflections[instance->GetType()] = this;
    }

    Type Reflection::GetType() const
    {
        return instance->GetType();
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
        return *reinterpret_cast<std::uintptr_t*>(instance);
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
