#pragma once

#include "Reflective.h"

namespace BDXKEngine
{
    class Reflection;

    class ReflectionTransferer : public Transferer
    {
    public:
        ReflectionTransferer(const Reflective& reflective, Reflection& reflection);
        bool IsValid() const;
    private:
        void PushPath(const std::string& key) override;
        void TransferValue(void* value, const Type& type) override;
        std::uintptr_t origin;
        int length;
        Reflection& reflection;
        std::string currentKey;
        std::string startKey;
    };


    class Reflection
    {
        friend ReflectionTransferer;
    public:
        template <typename Type>
        static const Reflection& GetReflection()
        {
            return GetReflection(GetTypeOf<Type>());
        }
        static const Reflection& GetReflection(const Type& id);
        static const Reflection& GetReflection(const Reflective* reflective);
        static int GetReflections(std::vector<Reflection*>& reflections,
                                  const std::function<bool(const Reflection&)>& condition = [](const Reflection&) { return true; }
        );

        Reflection(const std::function<Reflective*()>& constructor, int size);
        virtual ~Reflection() = default;

        Type GetType() const;
        int GetSize() const;
        Reflective* GetInstance() const;
        Reflective* GetConstruction() const;
        template <typename T>
        T* GetConstruction() const
        {
            return dynamic_cast<T*>(GetConstruction());
        }
        std::uintptr_t GetVirtualTable() const;
        void* GetField(Reflective* target, const std::string& key) const;
        template <typename T>
        T& GetFieldOf(Reflective* target, const std::string& key) const
        {
            return *static_cast<T*>(GetField(target, key));
        }
        int GetFields(Reflective* target, std::vector<std::string>& names, std::vector<void*>& values, std::vector<Type>& types) const;

        virtual bool IsReceivable(Reflective* reflective) const = 0;
        virtual bool IsType(const Type& type) const;
        template <typename T>
        bool IsTypeOf() const
        {
            return IsType(GetTypeOf<T>());
        }
    private:
        inline static std::unordered_map<Type, Reflection*> reflections = {}; //必须提前在头文件中定义，以便在触发宏时能确保容器已初始化

        int size = 0;
        Reflective* instance;
        std::function<Reflective*()> constructor;
        std::vector<std::string> fieldName;
        std::vector<int> fieldPos;
        std::vector<Type> fieldType;
    };

    template <class T>
    class Internal_Reflection : public Reflection
    {
    public:
        Internal_Reflection()
            : Reflection([] { return static_cast<Reflective*>(new T()); }, sizeof(T))
        {
        }
    private:
        bool IsReceivable(Reflective* reflective) const override
        {
            return dynamic_cast<T*>(reflective) != nullptr;
        }
    };

    //请确保注册类已继承Reflective并存在无参构造函数
#define CustomReflection(target) inline Internal_Reflection<target> CustomReflection##target = {};
    CustomReflection(Reflective)

    struct StaticConstructorRegister
    {
        StaticConstructorRegister(void (*staticConstructor)())
        {
            staticConstructor();
        }
    };

#define CustomStaticConstructor(Func) inline static StaticConstructorRegister staticConstructorRegister = {&(Func)};
}
