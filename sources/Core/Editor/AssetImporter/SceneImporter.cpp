#include "SceneImporter.h"
#include "Core/Runtime/Base/Object/Serialization/ObjectSerialization.h"
#include "Core/Runtime/Framework/Scene/Scene.h"
#include "Core/Runtime/Framework/Scene/SceneLibrary.h"

namespace BDXKEditor
{
    ObjectPtrBase SceneImporter::Import()
    {
        const std::string fileName = GetTargetPath().filename().string();
        //转储Assets数据到Resources
        auto serializer = ObjectSerialization::CreateJsonSerializer();
        const ObjectPtrBase objectPtrBase = ObjectSerialization::Load(GetTargetPath(), serializer, GetTargetGuid(), false);
        Scene::Save(objectPtrBase.ToObject<Scene>()); //用于运行时加载场景
        return objectPtrBase;
    }
}
