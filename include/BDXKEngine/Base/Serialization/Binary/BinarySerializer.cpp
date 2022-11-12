#include "BinarySerializer.h"
#include <sstream>

#include "BinaryReader.h"
#include "BinaryWriter.h"
#include "BDXKEngine/Base/Reflection/Reflection.h"

namespace BDXKEngine
{
    std::string BinarySerializer::Serialize(Reflective* input)
    {
        const std::shared_ptr<std::stringstream> stringstream(new std::stringstream());

        //获取数据
        const std::shared_ptr<BinaryWriter> binaryWriter(new BinaryWriter(stringstream));
        input->Transfer(*binaryWriter);

        return stringstream->str();
    }
    Reflective* BinarySerializer::Deserialize(std::string input)
    {
        const std::shared_ptr<std::stringstream> stringstream(new std::stringstream());
        const std::shared_ptr<BinaryReader> binaryReader(new BinaryReader(stringstream));

        //解析类型信息
        stringstream->str(input);
        const Type type = Reflective::GetTypeID(*binaryReader);

        //创建实例
        auto* output = Reflection::GetReflection(type).GetConstruction<Reflective>();

        //填充数据
        stringstream->str(input);
        output->Transfer(*binaryReader);

        return output;
    }
    Reflective* BinarySerializer::Clone(Reflective* input)
    {
        const std::shared_ptr<std::stringstream> stringstream(new std::stringstream());
        //获取数据
        const std::shared_ptr<BinaryWriter> binaryWriter(new BinaryWriter(stringstream));
        input->Transfer(*binaryWriter);

        //创建实例
        auto* output = Reflection::GetReflection(input).GetConstruction<Reflective>();

        //填充数据
        const std::shared_ptr<BinaryReader> binaryReader(new BinaryReader(stringstream));
        output->Transfer(*binaryReader);

        return output;
    }
}
