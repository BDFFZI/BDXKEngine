#include <iostream>
#include <vector>
#include "BDXKEngine/Base/Package/Map.h"
#include "BDXKEngine/Base/Reflection/Reflection.h"
#include "BDXKEngine/Base/Serializer/Core/Serializer.h"
#include "BDXKEngine/Platform/Serialization/SerializerExtension/Json/JsonExporter.h"
#include "BDXKEngine/Platform/Serialization/SerializerExtension/Json/JsonImporter.h"


int main(int argc, char* argv[])
{
    using namespace BDXKEngine;
    Reflection::SetReflection<Map<std::string, bool>>();
    std::string data = ReadFile("projectWindow.ini");

    // rapidjson::Document document = {};
    // document.Parse(data.c_str());
    // std::string a = document["keys"]["item0"].GetString();
    // std::string b = document["keys"]["item3"].GetString();

    JsonImporter importer = {};
    
    importer.Reset(data);
    const Type type = Reflective::GetType(importer);
    auto* result = Reflection::GetReflection(type).GetConstruction<Reflective>();
    importer.Reset(data);
    result->Transfer(importer);
    
    rapidjson::Document& document = importer.GetDocument();
    std::string a = document["keys"]["item0"].GetString();
    std::string b = document["keys"]["item3"].GetString();

    // const Serializer1<JsonImporter, JsonExporter> serializer = {};
    // Map<std::string, bool>* isUnfoldingPackage = dynamic_cast<Map<std::string, bool>*>(serializer.Deserialize(data));
    return 0;
}
