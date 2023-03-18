#include "Serializer.h"
#include <filesystem>
#include <fstream>
#include "BDXKEngine/Base/Reflection/Reflection.h"

namespace BDXKEngine
{
    std::string ReadFile(const std::string& path)
    {
        std::ifstream ifstream(path, std::ios_base::binary);
        if (ifstream.is_open() == false)throw std::exception("文件读取失败");

        const auto size = static_cast<int>(std::filesystem::file_size(path));
        const std::unique_ptr<char[]> buffer = std::unique_ptr<char[]>(new char[size]);
        ifstream.read(buffer.get(), size);
        ifstream.close();

        return {buffer.get(), static_cast<size_t>(size)};
    }
    void WriteFile(const std::string& path, const std::string& data)
    {
        std::ofstream ofstream(path, std::ios_base::binary);
        if (ofstream.is_open() == false)throw std::exception("文件写入失败");

        ofstream << data;
        ofstream.close();
    }

    Serializer::Serializer(IOTransferer& importer, IOTransferer& exporter):
        importer(importer), exporter(exporter)
    {
    }
    std::string Serializer::Serialize(Reflective* input) const
    {
        //获取数据
        std::string result;
        input->Transfer(exporter);
        exporter.Reset(result);

        return result;
    }
    Reflective* Serializer::Deserialize(std::string input) const
    {
        //解析类型信息
        importer.Reset(input);
        const Type type = Reflective::GetType(importer);

        //创建实例
        auto* result = Reflection::GetReflection(type).GetConstruction<Reflective>();

        //填充数据
        importer.Reset(input);
        result->Transfer(importer);

        return result;
    }
    Reflective* Serializer::Clone(Reflective* input) const
    {
        //导出数据
        input->Transfer(exporter);
        std::string data;
        exporter.Reset(data);

        //导入数据
        importer.Reset(data);
        Reflective* output = Reflection::GetReflection(input).GetConstruction();
        output->Transfer(importer);

        return output;
    }
}
