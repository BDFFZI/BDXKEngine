#pragma once
#include "Base/Object/Object.h"

//Activating影响修改Enabling的生效时间
//Enabling影响各种事件的执行能力

namespace BDXKEngine
{
    class ObjectSwitchable : public Object
    {
    public:
        virtual bool GetIsActivating() const =0;
        virtual bool GetIsEnabling() const;
        virtual void SetIsEnabling(bool state) =0;
        virtual void SetIsActivating(bool state) =0;
        bool IsActivatingAndEnabling() const;

        std::wstring ToString() override;
    protected:
        bool isEnabling = true;
    };
}
