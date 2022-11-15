#include <cstdio>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <fstream>
#include "BDXKEngine/Base/Object/ObjectSerializer.h"
#include "BDXKEngine/Base/Object/Core/ObjectPtr.h"
#include "BDXKEngine/Base/Object/Core/Object.h"
#include "BDXKEngine/Base/Reflection/Reflection.h"
#include "BDXKEngine/Base/Serialization/Binary/BinaryExporter.h"
#include "BDXKEngine/Base/Serialization/Binary/BinaryImporter.h"

using namespace BDXKEngine;

class Assets : public Object
{
public:
    void Transfer(Transferer& transferer) override
    {
        Object::Transfer(transferer);

        TransferFieldInfo(intValue);
    }

    int intValue = 66;
};

CustomReflection(Assets)

class Container : public Object
{
public:
    void Transfer(Transferer& transferer) override
    {
        Object::Transfer(transferer);

        TransferFieldInfo(value);
        TransferFieldInfo(assets);
    }

    void MarkAwake() override
    {
        Object::MarkAwake();
        
        Object::MarkAwake(assets);
    }

    int value = 99;
    ObjectPtr<Assets> assets;
};

CustomReflection(Container)

int main()
{
    //TODO Object要支持序列化Object功能

    std::setlocale(LC_ALL, "zh-CN.UTF-8");

    {
        ObjectSerializer<BinaryExporter, BinaryImporter> serializer;

        //导出物体
        ObjectPtr assets = Object::Instantiate(new Assets{});
        ObjectPtr container = Object::Instantiate(new Container{});
        container->assets = assets;
        std::string data = serializer.Serialize(container.ToObjectBase());
        std::ofstream ofstream("C:/Users/BDFFZI/Desktop/data.bin");
        ofstream << data;
        ofstream.close();

        //导入物体
        int size = std::filesystem::file_size("C:/Users/BDFFZI/Desktop/data.bin");
        std::ifstream ifstream("C:/Users/BDFFZI/Desktop/data.bin", std::ifstream::binary);
        char* buffer = new char[size];
        ifstream.read(buffer, size);
        ifstream.close();
        Object* reflective = dynamic_cast<Object*>(serializer.Deserialize(std::string(buffer, size)));
        ObjectPtr<Container> object = Object::Instantiate<Container>(dynamic_cast<Container*>(reflective), serializer);
        object = Object::Instantiate<Container>(object, serializer);

        auto a = Object::GetAllObjects();
        ObjectPtrBase::PrintRefCountMap();
    }

    auto b = Object::GetAllObjects();
    ObjectPtrBase::PrintRefCountMap();

    getchar();
}
