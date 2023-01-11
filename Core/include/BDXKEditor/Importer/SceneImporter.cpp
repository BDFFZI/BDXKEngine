#include "SceneImporter.h"
#include "BDXKEngine/Framework/Scene.h"
#include "BDXKEngine/Function/Resources/Resources.h"
#include "BDXKEngine/Platform/Serialization/Serialization.h"

namespace BDXKEditor
{
    ObjectPtrBase SceneImporter::ImportAsset(const std::string& filePath)
    {
        //转储Assets数据到Resources
        auto serializer = Serialization::CreateJsonSerializer();
        const ObjectPtrBase objectPtrBase = Serialization::Load(filePath, serializer, false);
        Resources::Save(objectPtrBase->GetName(), objectPtrBase);

        Scene::Load(objectPtrBase->GetName(), true);
        return static_cast<ObjectPtrBase>(Scene::GetCurrentScene());
    }
}
