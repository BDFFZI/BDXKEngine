#pragma once
#include "BDXKEngine/Base/Object/Object.h"

//Activating影响修改Enabling的生效时间
//Enabling影响各种事件的执行能力

namespace BDXKEngine
{
    class SwitchableObject : public Object
    {
    public:
        virtual bool GetIsEnabling() const;
        virtual bool GetIsActivating() const =0;
        virtual void SetIsEnabling(bool state) =0;

        bool IsActivatingAndEnabling() const;

        std::string ToString() override;
    protected:
        bool isEnabling = true;

        void UpdateActivating();
        virtual void OnUpdateActivating(bool state) =0;

        void Transfer(Transferrer& transferrer) override;
    };
}
