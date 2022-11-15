#include "Serializer.h"

#include "BDXKEngine/Base/Reflection/Reflection.h"

namespace BDXKEngine
{
    Serializer::Serializer(Importer& importer, Exporter& exporter):
        importer(importer), exporter(exporter)
    {
    }
    std::string Serializer::Serialize(Reflective* input)
    {
        //获取数据
        std::string result;
        input->Transfer(exporter);
        exporter.Export(result);

        return result;
    }
    Reflective* Serializer::Deserialize(std::string input)
    {
        //解析类型信息
        importer.Import(input);
        const Type type = Reflective::GetTypeID(importer);

        //创建实例
        auto* result = Reflection::GetReflection(type).GetConstruction<Reflective>();

        //填充数据
        importer.Import(input);
        result->Transfer(importer);

        return result;
    }
    Reflective* Serializer::Clone(Reflective* input)
    {
        //导出数据
        input->Transfer(exporter);
        std::string data;
        exporter.Export(data);

        //导入数据
        importer.Import(data);
        Reflective* output = Reflection::GetReflection(input).GetConstruction();
        output->Transfer(importer);

        return output;
    }
}
