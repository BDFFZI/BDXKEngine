#pragma once
#include <map>

namespace BDXKEngine
{
    class ObjectBase;

    class ObjectPtrBase
    {
    public:
        static void PrintRefCountMap();

        ObjectPtrBase();
        ObjectPtrBase(const ObjectBase* object);
        ObjectPtrBase(const ObjectPtrBase& objectPtr);
        virtual ~ObjectPtrBase();

        //提取内容
        int GetInstanceID() const;

        bool IsNull() const;
        bool IsNotNull() const;

        ObjectBase* ToObjectBase() const;
        template <typename TObject>
        TObject* ToObject() const
        {
            return dynamic_cast<TObject*>(ToObjectBase());
        }

        bool operator ==(const ObjectPtrBase& other) const;
        bool operator !=(const ObjectPtrBase& other) const;
        ObjectPtrBase& operator=(const ObjectPtrBase& objectPtr);

    protected:
        static std::map<int, int> refCountMap;

        int instanceID;

        virtual void AddRef(int refInstanceID);
        virtual void RemoveRef();
    };
}
