#include <cstdio>
#include <iostream>
#include <filesystem>
#include "BDXKEngine/Base/BDXKObject/BDXKObject.h"
#include "BDXKEngine/Base/Object/ObjectSerializer.h"
#include "BDXKEngine/Base/Object/Core/ObjectPtr.h"
#include "BDXKEngine/Base/Object/Core/Object.h"
#include "BDXKEngine/Base/Reflection/Reflection.h"
#include "BDXKEngine/Base/Serialization/Binary/BinaryExporter.h"

using namespace BDXKEngine;

class Assets : public Object
{
public:
    void Transfer(Transferer& transferer) override
    {
        Object::Transfer(transferer);

        TransferFieldInfo(intValue);
        TransferFieldInfo(rect);
    }

    int intValue = 66;
    Rect rect = {0, 0, 30, 60};
};

CustomReflection(Assets)

class Container : public Object
{
public:
    void Transfer(Transferer& transferer) override
    {
        Object::Transfer(transferer);

        TransferFieldInfo(value);
        TransferFieldInfo(position);
        TransferFieldInfo(assets);
        TransferFieldInfo(container);
    }

    int value = 99;
    Vector3 position = {7, 6.6f, 10.23f};
    ObjectPtr<Assets> assets;
    ObjectPtr<Container> container;
};

CustomReflection(Container)

int main()
{
    //TODO 指针指向自己会导致无法自动回收
    //TODO 持久化信息需要保存

    std::setlocale(LC_ALL, "zh-CN.UTF-8");

    {
        Guid guid;

        //导出物体
        {
            ObjectPtr assets = Object::Instantiate(new Assets{});
            ObjectPtr container = Object::Instantiate(new Container{});
            ObjectPtr container2 = Object::Instantiate(new Container{});
            container->assets = assets;
            container->container = container2;
            container2->container = container2;
            guid = BDXKObject::Save(container.ToObjectBase());
        }

        auto a = Object::GetAllObjects();
        ObjectPtrBase::PrintRefCountMap();

        //导入物体
        {
            ObjectPtr prefab = BDXKObject::Load(guid).ToObject<Container>();
            ObjectPtr<Container> object = BDXKObject::Instantiate<Container>(prefab);
            object = BDXKObject::Instantiate<Container>(object);
        }
    }

    auto b = Object::GetAllObjects();
    ObjectPtrBase::PrintRefCountMap();

    getchar();
}
