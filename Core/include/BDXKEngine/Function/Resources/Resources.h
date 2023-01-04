#pragma once
#include "BDXKEngine/Base/Object/Serializer/ObjectSerializer.h"

namespace BDXKEngine
{
    class Resources
    {
    public:
        static ObjectPtrBase Load(const std::string& path, bool retainPersistent = true);
        template <typename TObject>
        static ObjectPtr<TObject> Load(const std::string& path, bool retainPersistent = true)
        {
            return Load(path, retainPersistent).ToObject<TObject>();
        }
        static void Save(const std::string& path, const ObjectPtrBase& objectPtr);

        static bool IsExisting(const std::string& path);

        static ObjectPtrBase FindSerializationFallback(const Guid& guid);
    private:
        static void StaticConstructor();
        CustomStaticConstructor(StaticConstructor)
    };
}
