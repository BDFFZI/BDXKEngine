#pragma once
#include "ObjectSwitchable.h"
#include "Base/Object/ObjectPtr.h"

namespace BDXKEngine
{
    class GameObjectManager;
    class GameObject;
    class Transform;

    //确保TComponent有默认构造函数
    class Component : public ObjectSwitchable
    {
        friend GameObject;
        friend GameObjectManager;
    public:
        //设为虚函数是为了转为多态类型
        ObjectPtr<GameObject> GetGameObject();
        ObjectPtr<Transform> GetTransform();
        bool GetIsActivating() const override;
        void SetIsEnabling(bool state) override;

    protected:
        //以下函数重写时请务必回调父类函数
        void Transfer(Transferrer& transferrer) override;
        void Awake() override;
        void Destroy() override;
        virtual void Enable();
        virtual void Disable();
    private:
        ObjectPtr<GameObject> gameObject = nullptr;

        void OnUpdateActivating(bool state) override;
    };
}
