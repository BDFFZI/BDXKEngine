#pragma once
#include "BDXKEngine/Base/Object/ObjectPtr.h"
#include "BDXKEngine/Framework/Object/ScriptableObject.h"

namespace BDXKEngine
{
    class Transform;
    class GameObject;

    //确保TComponent有默认构造函数
    class Component : public ScriptableObject
    {
        friend GameObject;
    public:
        //设为虚函数是为了转为多态类型
        ObjectPtr<GameObject> GetGameObject();
        ObjectPtr<Transform> GetTransform();
        bool GetIsActivating() const override;
        void Transfer(transferer& transferer) override;
    protected:
        void Destroy() override;
    private:
        ObjectPtr<GameObject> gameObject = nullptr;
    };
}
