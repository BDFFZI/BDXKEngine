#pragma once
#include "BDXKEngine/Base/Object/ObjectPtr.h"
#include "BDXKEngine/Framework/ScriptableObject/ScriptableObject.h"

namespace BDXKEngine
{
    class GameObjectManager;
    class GameObject;
    class Transform;

    //确保TComponent有默认构造函数
    class Component : public ScriptableObject
    {
        friend GameObject;
        friend GameObjectManager;
    public:
        //设为虚函数是为了转为多态类型
        ObjectPtr<GameObject> GetGameObject();
        ObjectPtr<Transform> GetTransform();
        bool GetIsActivating() const override;
    protected:
        //以下函数重写时请务必回调父类函数
        void Transfer(Transferrer& transferrer) override;
        void Awake() override;
        void Destroy() override;
    private:
        ObjectPtr<GameObject> gameObject = nullptr;
    };
}
