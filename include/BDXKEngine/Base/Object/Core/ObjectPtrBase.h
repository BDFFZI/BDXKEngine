#pragma once
#include <map>

namespace BDXKEngine
{
    class Object;

    class ObjectPtrBase
    {
    public:
        static void PrintRefCountMap();

        ObjectPtrBase();
        ObjectPtrBase(const Object* object);
        ObjectPtrBase(const ObjectPtrBase& objectPtr);
        virtual ~ObjectPtrBase(); //为了序列化所以不能使用虚函数功能（虚表指针无法初始化）

        int GetInstanceID() const;

        bool IsNull() const;
        bool IsNotNull() const;

        Object* ToObjectBase() const;
        template <typename TObject>
        TObject* ToObject() const
        {
            return dynamic_cast<TObject*>(ToObjectBase());
        }

        virtual Object* operator->() const;
        bool operator ==(const ObjectPtrBase& other) const;
        bool operator !=(const ObjectPtrBase& other) const;
        ObjectPtrBase& operator=(const ObjectPtrBase& objectPtr);
    protected:
        inline static std::map<int, int> refCountMap = {};

        int instanceID = 0;

        virtual void AddRef(int refInstanceID);
        virtual void RemoveRef();
    };
}
