#pragma once
#include "Core/Runtime/Base/Object/Object.h"
#include "Core/Runtime/Base/Reflection/Reflection.h"

namespace BDXKEngine
{
    class SwitchableObject : public Object
    {
    public:
        bool GetIsEnabling() const;
        void SetIsEnabling(bool state);
        bool IsNotResource() const;
        bool IsActivatingAndEnabling() const;

        virtual bool GetIsActivating() const;
        void UpdateActivating(); //通常用于在Enable和Disable时调用，以通知子物体更新状态

        void Transfer(Transferer& transferer) override;

    protected:
        virtual void Enable();
        virtual void Disable();

        void Awake() override;
        void Start() override;
        void Destroy() override;

    private:
        bool lastIsActivating = true; //上一次的激活状态

        bool isEnabling = true;
    };

    CustomReflection(SwitchableObject)
}
