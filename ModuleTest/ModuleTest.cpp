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
    Assets() = default;

    Assets(const ObjectPtr<Object>& parent): parent(parent)
    {
    }

    void Transfer(Transferer& transferer) override
    {
        Object::Transfer(transferer);

        TransferFieldInfo(parent);
        TransferFieldInfo(intValue);
        TransferFieldInfo(rect);
    }

    ObjectPtr<Object> parent = nullptr;
    int intValue = 66;
    Rect rect = {0, 0, 30, 60};
};

CustomReflection(Assets)

class Container : public Object
{
public:
    Container() = default;

    Container(const ObjectPtr<Object>& parent): parent(parent)
    {
    }

    void Transfer(Transferer& transferer) override
    {
        Object::Transfer(transferer);

        TransferFieldInfo(parent);
        TransferFieldInfo(value);
        TransferFieldInfo(position);
        TransferFieldInfo(assets);
    }

    void Awake() override
    {
        Object::Awake();


        if (assets != nullptr && assets->parent != this) assets = BDXKObject::Instantiate(assets);
    }

    ObjectPtr<Object> parent = nullptr;
    int value = 99;
    Vector3 position = {7, 6.6f, 10.23f};
    ObjectPtr<Assets> assets = nullptr;
};

CustomReflection(Container)

int main()
{
    std::setlocale(LC_ALL, "zh-CN.UTF-8");

    {
        std::string path = "C:/Users/BDFFZI/Desktop/BDXKTemp/scene.json";

        ObjectPtr<Container> prefab;
        //导入物体
        {
            prefab = BDXKObject::Load(path).ToObject<Container>();
            ObjectPtr<Container> instance = BDXKObject::Instantiate<Container>(prefab);
        }

        auto a = Object::GetAllObjects();
        ObjectPtrBase::PrintRefCountMap();

        //导出物体
        {
            prefab->value += 1;
            BDXKObject::Save(path, prefab);
        }
    }

    auto b = Object::GetAllObjects();
    ObjectPtrBase::PrintRefCountMap();

    getchar();
}
