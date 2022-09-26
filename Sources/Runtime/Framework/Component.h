#pragma once
#include "ISwitchable.h"
#include "Base/Object/ObjectPtr.h"

namespace BDXKEngine
{
    class GameObjectManager;
    class GameObject;
    class Transform;

    //确保TComponent有默认构造函数
    class Component : public Object, public ISwitchable
    {
        friend GameObject;
        friend GameObjectManager;
    public:
        //设为虚函数是为了转为多态类型
        ObjectPtr<GameObject> GetGameObject();
        ObjectPtr<Transform> GetTransform();

        bool GetIsActivating() const override;
        bool GetIsEnabling() const override;
        void SetIsEnabling(bool state) override;
    protected:
        void Export(Exporter& exporter) override;
        void Import(Importer& importer) override;
        //以下函数重写时请务必回调父类函数
        void Awake() override;
        void Destroy() override;
        virtual void Enable();
        virtual void Disable();
    private:
        ObjectPtr<GameObject> gameObject = nullptr;
        bool isEnabling = true;

        void SetIsActivating(bool state) override;
    };
}
