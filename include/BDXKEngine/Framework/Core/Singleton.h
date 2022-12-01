#pragma once
#include "BDXKEngine/Base/Object/Object.h"

namespace BDXKEngine
{
    template <typename TSingleton>
    class Singleton : public Object
    {
    public:
        static TSingleton& GetSingleton()
        {
            if (singleton.IsNull())TSingleton::Reset();
            return *singleton.template ToObject<TSingleton>();
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
