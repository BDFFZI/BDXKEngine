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

        void SetIsActivating(); // 用于激活信号的传递，在OnSetIsEnabling中使用它
        void SetIsEnabling(bool state);

        bool IsActivatingAndEnabling() const;
        
        void Transfer(transferer& transferer) override;
        std::string ToString() override;
    protected:
        bool isAwakened = false;
        bool isEnabling = true;
        
        virtual void Enable();
        virtual void Disable();
        void Awake() override;
        void PreDestroy() override;
    };
}
