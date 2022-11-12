#include <cstdio>
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>

#include "BDXKEngine/Base/Object/ObjectPtr.h"
#include "BDXKEngine/Base/Object/Core/Object.h"
#include "BDXKEngine/Base/Reflection/Reflection.h"
#include "BDXKEngine/Base/Serialization/Binary/BinaryReader.h"
#include "BDXKEngine/Base/Serialization/Binary/BinarySerializer.h"


using namespace BDXKEngine;

enum class Level
{
    None = 0,
    Low = 1,
    Middle = 2,
    Height = 3,
};

class Assets : public Object
{
public:
    Assets()
    {
    }
    Assets(int intValue, Level enumValue)
        : intValue(intValue),
          enumValue(enumValue)
    {
    }

    void Transfer(Transferer& transferer) override
    {
        Object::Transfer(transferer);

        TransferFieldInfo(intValue);
        TransferFieldInfoOf(enumValue, int);
    }

    int intValue = 0;
    Level enumValue = Level::None;
    ObjectPtr<Assets> child;
};

CustomReflection(Assets)

int main()
{
    //TODO Object要支持序列化Object功能
    //TODO 外部接口全面使用ObjectPtrBase代替
    //TODO 考虑将ObjectManager整合进Object内部

    std::setlocale(LC_ALL, "zh-CN.UTF-8");

    {
        BinarySerializer binarySerializer;
        //导出物体

        {
            ObjectPtr target = Object::Instantiate(new Assets{23, Level::Height}, binarySerializer);
            std::string data = binarySerializer.Serialize(target.ToObjectBase());
            std::ofstream ofstream("C:/Users/BDFFZI/Desktop/data.bin");
            ofstream << data;
            ofstream.close();
        }


        ObjectPtrBase::PrintRefCountMap();


        {
            int size = std::filesystem::file_size("C:/Users/BDFFZI/Desktop/data.bin");
            std::ifstream ifstream("C:/Users/BDFFZI/Desktop/data.bin", std::ifstream::binary);
            char* buffer = new char[size];
            ifstream.read(buffer, size);
            ifstream.close();

            Assets* sourceAssets = dynamic_cast<Assets*>(binarySerializer.Deserialize(std::string(buffer, size)));
            ObjectPtr<Assets> assets = Object::Instantiate(sourceAssets, binarySerializer);
            ObjectPtr<Assets> assets2 = assets;
            assets = nullptr;
            ObjectPtrBase::PrintRefCountMap();
            
            std::cout << assets2->intValue << std::endl;
            std::cout << static_cast<int>(assets2->enumValue) << std::endl;
        }
    }
    ObjectPtrBase::PrintRefCountMap();


    getchar();
}
