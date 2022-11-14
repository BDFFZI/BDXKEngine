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

class Container: public Object
{
public:
    void Transfer(Transferer& transferer) override
    {
        Object::Transfer(transferer);

        TransferFieldInfo(value);
        TransferFieldInfo(assets);
    }

    int value = 99;
    ObjectPtr<Assets> assets;
};

CustomReflection(Container)

int main()
{
    //TODO Object要支持序列化Object功能

    std::setlocale(LC_ALL, "zh-CN.UTF-8");
    
    ObjectPtr assets = Object::Instantiate(new Assets{});
    ObjectPtr container = Object::Instantiate(new Container{});
    container->assets = assets;

    ObjectSerializer serializer;


    //导出物体
    std::string data = serializer.Serialize(container.ToObjectBase());
    std::ofstream ofstream("C:/Users/BDFFZI/Desktop/data.bin");
    ofstream << data;
    ofstream.close();
    //
    //
    // int size = std::filesystem::file_size("C:/Users/BDFFZI/Desktop/data.bin");
    // std::ifstream ifstream("C:/Users/BDFFZI/Desktop/data.bin", std::ifstream::binary);
    // char* buffer = new char[size];
    // ifstream.read(buffer, size);
    // ifstream.close();
    //
    // Assets* sourceAssets = dynamic_cast<Assets*>(serializer.Deserialize(std::string(buffer, size)));
    // ObjectPtr<Assets2>  = Object::Instantiate(sourceAssets);


    getchar();
}
