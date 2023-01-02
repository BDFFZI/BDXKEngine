#pragma once
#include "BDXKEngine/Base/Object/Object.h"

namespace BDXKEngine
{
    template <typename TSingleton>
    class Singleton : public Object
    {
    public:
        static ObjectPtr<TSingleton>& GetSingleton()
        {
            if (singleton == nullptr)
                Create<TSingleton>();
            return singleton;
        }
    protected:
        static ObjectPtr<TSingleton> singleton;

        void Awake() override
        {
            Object::Awake();

            if (singleton.IsNotNull() && singleton.GetInstanceID() != GetInstanceID())
                DestroyImmediate(singleton);
            singleton = dynamic_cast<TSingleton*>(this);
        }
    };

    template <typename TSingleton>
    ObjectPtr<TSingleton> Singleton<TSingleton>::singleton = nullptr;
}
