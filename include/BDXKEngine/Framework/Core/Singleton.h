#pragma once
#include "BDXKEngine/Base/Object/Core/Object.h"

namespace BDXKEngine
{
    template <typename TSingleton>
    class Singleton : public Object
    {
    public:
        static const ObjectPtr<TSingleton>& GetSingleton()
        {
            if (singleton.IsNull())
                throw std::exception("该单例尚未创建");
            return singleton;
        }
    private:
        static ObjectPtr<TSingleton> singleton;
        
        void Awake() override
        {
            Object::Awake();

            if (singleton.IsNotNull()) DestroyImmediate(singleton);
            singleton = dynamic_cast<TSingleton*>(this);
        }
    };

    template <typename TSingleton>
    ObjectPtr<TSingleton> Singleton<TSingleton>::singleton = nullptr;
}
