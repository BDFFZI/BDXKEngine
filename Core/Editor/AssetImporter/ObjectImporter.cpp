#include "ObjectImporter.h"

#include "Core/Runtime/Base/Object/Serialization/ObjectSerialization.h"

namespace BDXKEditor
{
    ObjectPtrBase ObjectImporter::Import()
    {
        //Serialization会自动处理FileID和Object替换，不需要人为绑定

        auto serializer = ObjectSerialization::CreateJsonSerializer();
        return ObjectSerialization::Load(GetTargetPath(), serializer, GetTargetGuid(), false);
    }
}
