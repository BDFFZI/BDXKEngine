#pragma once
#include "BDXKEngine/Base/Object/Object.h"
#include "SwitchableObjectEvent.h"

//Activating影响Enabling的生效能力
//Enabling影响各种事件的执行能力

namespace BDXKEngine
{
    class SwitchableObject : public Object
    {
    public:
        virtual bool GetIsActivating() const;
        bool GetIsEnabling() const;

        void SetIsActivating(); //一个特殊的函数，仅由子类自行调用，以在合适的时候触发事件
        void SetIsEnabling(bool state);

        bool IsActivatingAndEnabling() const;
        std::string ToString() override;
    protected:
        bool isAwakened = false;
        bool isEnabling = true;

        virtual void Enable();
        virtual void Disable();
        void Awake() override;
        void Destroy() override;
        void Transfer(Transferrer& transferrer) override;
    };
}
