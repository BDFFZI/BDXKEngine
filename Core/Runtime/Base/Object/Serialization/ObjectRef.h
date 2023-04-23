#pragma once
#include <unordered_map>
#include <unordered_set>
#include <crossguid/guid.hpp>
#include "Core/Runtime/Base/Reflection/Transferer.h"


namespace BDXKEngine
{
    struct ObjectRef;
}

template <>
struct std::hash<BDXKEngine::ObjectRef>
{
    size_t operator()(BDXKEngine::ObjectRef const& objectRef) const noexcept;
};


namespace BDXKEngine
{
    typedef size_t FileID;
    typedef unsigned int ID;

    //不保证ObjectID的时效性，注意判断空值
    struct ObjectRef
    {
        static ObjectRef Null();
        static FileID GenerateFileID(xg::Guid guid);

        static std::unordered_map<ObjectRef, ID>& GetAll();
        static ObjectRef GetObjectRef(ID objectID);
        static ID GetObjectID(ObjectRef objectRef); //当检索到的instance已不存在时也会返回0

        static ID GetRootObjectID(const xg::Guid& guid);
        static void MarkRoot(const ObjectRef& objectRef);
        static void MarkRoot(const ID& id);
        static bool IsRoot(const ObjectRef& objectRef);

        static void Add(ObjectRef objectRef, ID objectID); //当guid未绑定或旧的绑定instance已不存在时可用
        static void Remove(ID objectID);
        static void Remove(ObjectRef objectRef);
        static void Remove(xg::Guid guid); //所有使用该guid的ObjectRef都会被清除

        bool operator==(const ObjectRef& object) const = default;
        bool operator!=(const ObjectRef& object) const = default;

        bool IsNull() const;

        xg::Guid guid;
        FileID fileID = 0;

        void Transfer(Transferer& transferer);
        std::string ToString() const;

    private:
        inline static std::unordered_map<ID, ObjectRef> objectIDToObjectRef = {};
        inline static std::unordered_map<ObjectRef, ID> objectRefToObjectID = {};
        inline static std::unordered_map<xg::Guid, std::unordered_set<FileID>> guidToFileIDs = {};
        inline static std::unordered_map<xg::Guid, ID> guidToRootObjectID = {};
    };
}
