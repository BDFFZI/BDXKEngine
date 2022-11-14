#pragma once
#include "Reflective.h"

namespace BDXKEngine
{
    class Reflection;

    class ReflectionTransferer : public Transferer
    {
    public:
        ReflectionTransferer(const Reflective& reflective, Reflection& reflection);

        void PushPath(const std::string& key) override;
        void TransferValueFallback(void* value, const Type& typeID) override;

        std::uintptr_t origin;
        int length;
        Reflection& reflection;
        std::string currentKey;
    };

    class Reflection
    {
        friend ReflectionTransferer;
    public:
        template <typename Type>
        static const Reflection& GetReflection()
        {
            return GetReflection(GetTypeID<Type>());
        }
        static const Reflection& GetReflection(const Type& id);
        static const Reflection& GetReflection(const Reflective* reflective);

        Reflection() = default;
        Reflection(const std::function<Reflective*()>& constructor, int size);

        Reflective* GetConstruction() const;
        void* GetField(Reflective* target, const std::string& key) const;

        template <typename T>
        T* GetConstruction() const
        {
            return dynamic_cast<T*>(GetConstruction());
        }
        template <typename T>
        T& GetFieldOf(Reflective* target, const std::string& key) const
        {
            return *static_cast<T*>(GetField(target, key));
        }
    private:
        inline static std::unordered_map<Type, Reflection*> reflections = {}; //必须提前在头文件中定义，以便在触发宏时能确保容器已初始化

        int size = 0;
        std::function<Reflective*()> constructor;
        std::unordered_map<std::string, int> fields;
    };

    //请确保注册类已继承Reflective并存在无参构造函数
#define CustomReflection(target) inline Reflection CustomReflection##target([] {return static_cast<Reflective*>(new target());},sizeof(target));
    CustomReflection(Reflective)
}
