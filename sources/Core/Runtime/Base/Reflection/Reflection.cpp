#include "Reflection.h"
#include <ranges>

namespace BDXKEngine
{
    Type fieldType = GetTypeOf<char>();

    ReflectionTransferer::ReflectionTransferer(const Reflective& reflective, Reflection& reflection):
        reflection(reflection)
    {
        origin = reinterpret_cast<std::uintptr_t>(&reflective);
        length = reflection.size;
    }
    bool ReflectionTransferer::IsValid() const
    {
        return startKey == "type";
    }

    void ReflectionTransferer::PushPath(const std::string& key)
    {
        currentKey = key;
        if (startKey.empty())
            startKey = key;
    }
    std::function<void(void*, const Type&)> ReflectionTransferer::GetTransferFunc(const Type& type)
    {
        return [this](void* field, const Type& realType)
        {
            const int addressOffset = static_cast<int>(reinterpret_cast<std::uintptr_t>(field) - origin);
            if (addressOffset >= 0 && addressOffset < length)
            {
                this->reflection.fieldNames.emplace_back(currentKey);
                this->reflection.fieldPositions.emplace_back(addressOffset);
                this->reflection.fieldTypes.emplace_back(realType);
            }
        };
    }

    bool Reflection::HasReflection(const Type& id)
    {
        return reflections.contains(id);
    }
    const Reflection& Reflection::GetReflection(const Type& id)
    {
        const auto& item = reflections.find(id);
        if (item == reflections.end())throw std::exception((id + "的反射信息未定义").c_str());

        return *item->second;
    }
    const Reflection& Reflection::GetReflection(const Reflective* reflective)
    {
        return GetReflection(reflective->GetType());
    }
    int Reflection::GetReflections(std::vector<Reflection*>& reflections, const std::function<bool(const Reflection&)>& condition)
    {
        reflections.clear();
        for (const auto& item : Reflection::reflections | std::ranges::views::values)
        {
            if (condition(*item) == true)
                reflections.emplace_back(item);
        }

        return static_cast<int>(reflections.size());
    }

    Reflection::Reflection(const std::function<Reflective*()>& constructor, const int size)
    {
        //获取实例
        instance = constructor();
        if (HasReflection(instance->GetType()))
        {
            delete instance;
            return;
        }

        //获取类型大小
        this->size = size;
        //获取构造函数
        this->constructor = constructor;
        //获取字段信息
        ReflectionTransferer reflecttransferer = {*instance, *this};
        instance->Transfer(reflecttransferer);
        if (reflecttransferer.IsValid() == false)throw std::exception("注册类不符合反射规范(未调用父类Transfer函数)");
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
    Reflective* Reflection::GetInstance() const
    {
        return instance;
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
        const auto& item = std::ranges::find(fieldNames, key);
        if (item == fieldNames.end())
            throw std::exception((key + "的字段信息未定义").c_str());

        return reinterpret_cast<char*>(target) + fieldPositions[(item - fieldNames.begin())];
    }
    int Reflection::GetFields(Reflective* target, std::vector<std::string>& names, std::vector<void*>& values,
                              std::vector<Type>& types) const
    {
        names.clear();
        values.clear();
        types.clear();
        const int count = static_cast<int>(fieldNames.size());
        for (int i = 0; i < count; i++)
        {
            names.emplace_back(fieldNames.at(i));
            values.emplace_back(reinterpret_cast<char*>(target) + fieldPositions.at(i));
            types.emplace_back(fieldTypes.at(i));
        }

        return count;
    }
    bool Reflection::CanConvertTo(const Type& type) const
    {
        return reflections[type]->CanReceiveFrom(instance);
    }
}
