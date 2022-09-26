#pragma once

//Activating影响修改Enabling的生效时间
//Enabling影响各种事件的执行能力

namespace BDXKEngine
{
    struct ISwitchable
    {
        virtual ~ISwitchable() = default;
        virtual bool GetIsActivating() const =0;
        virtual bool GetIsEnabling() const =0;
        virtual void SetIsEnabling(bool state) =0;
        virtual void SetIsActivating(bool state) =0;
        bool IsActivatingAndEnabling() const;
    };
}
