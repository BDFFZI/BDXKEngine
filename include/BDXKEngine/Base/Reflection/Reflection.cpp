#include "Reflection.h"

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
    void ReflectionTransferer::TransferValue(void* value, const Type& typeID)
    {
        const int addressOffset = static_cast<int>(reinterpret_cast<std::uintptr_t>(value) - origin);
        if (addressOffset >= 0 && addressOffset < length)reflection.fields[currentKey] = addressOffset;
    }

    const Reflection& Reflection::GetReflection(const Type& id)
    {
        const auto& item = reflections.find(id);
        if (item == reflections.end())throw std::exception((id + "的反射信息未定义").c_str());

        return *reflections[id];
    }
    const Reflection& Reflection::GetReflection(const Reflective* reflective)
    {
        return GetReflection(reflective->GetTypeID());
    }

    Reflection::Reflection(const std::function<Reflective*()>& constructor, int size)
    {
        Reflective* reflective = constructor();

        //获取类型大小
        this->size = size;
        //获取构造函数
        this->constructor = constructor;
        //获取字段信息
        ReflectionTransferer reflecttransferer = {*reflective, *this};
        reflective->Transfer(reflecttransferer);
        //注册反射
        reflections[reflective->GetTypeID()] = this;

        delete reflective;
    }

    Reflective* Reflection::GetConstruction() const
    {
        return constructor();
    }
    void* Reflection::GetField(Reflective* target, const std::string& key) const
    {
        const auto& item = fields.find(key);
        if (item == fields.end()) throw std::exception((key + "的字段信息未定义").c_str());

        return reinterpret_cast<char*>(target) + item->second;
    }
}
