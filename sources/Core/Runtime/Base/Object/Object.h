#pragma once
#include <map>
#include <ranges>
#include <vector>
#include <crossguid/guid.hpp>

#include "Core/Runtime/Base/Reflection/Reflection.h"
#include "Core/Runtime/Base/Reflection/Reflective.h"
#include "Pointer/ObjectPtr.h"

namespace BDXKEngine
{
    /// <summary>
    /// 创建：
    /// 通过InstantiateNoAwake加载未实例化的物体
    /// 在Import中通过Importer提供的函数嵌套加载子物体
    /// 然后再利用MarkActivating嵌套标记要唤醒的物体
    /// 最后通过FlushActivateBuffer一并唤醒
    /// 
    /// 删除：
    /// Destory只是标记为删除中状态
    /// 最后系统调用FlushDestroyBuffer来使所有被标记的物体删除
    /// DestroyImmediate是立即删除
    /// </summary>
    class Object : public Reflective
    {
    public:
        template <class TObject>
        static ObjectPtr<TObject> Create(const std::string& name = {})
        {
            ObjectPtr object = new TObject{};
            object->name = name;
            AwakeObject(object);
            return object;
        }
        static ObjectPtrBase Create(const Type& type);

        static void AwakeObject(const ObjectPtrBase& objectPtr); //唤醒。使用序列化功能创建的物体默认都是原生状态，需要唤醒，也是因为序列化所以需要用Awake代替构造函数
        static void DestroyObject(const ObjectPtrBase& object); //销毁
        //从ObjectID层面进行替换一般用于实现反序列化时的物体重新加载功能，使用替换功能时被替换的物体将被直接销毁。
        //TODO 替换功能（预制体）
        // 默认销毁方式会触发事件，由于GameObject会在事件中额外销毁子物体，导致替换好的子物体会被意外删除，所以没有人敢用
        // 但必须要触发事件，不然部分该销毁的子物体将会失去控制，导致内存泄漏
        // 目前采用先完全删除旧物体，再替换的方式，这又导致替换事件接收到的旧物体一种都是空的
        // 在Unity中这个需求涉及到预制体功能，实现方式是将差异数据写在序列化文件中，但我们的游戏引擎是不保存这些信息的，所以做不到
        //TODO
        static void ReplaceObject(ObjectPtrBase& newObject, ID targetObjectID);

        //搜索
        static const std::map<ID, Object*>& GetAllObjects();
        template <typename TObject>
        static std::vector<TObject*> FindObjectsOfType()
        {
            std::vector<TObject*> result{};
            for (auto& value : allObjects | std::views::values)
            {
                TObject* object = dynamic_cast<TObject*>(value);
                if (object != nullptr)result.emplace_back(object);
            }

            return result;
        }
        static Object* FindObjectOfObjectID(ID objectID);
        template <typename TObject>
        static TObject* FindObjectOfObjectID(const ID objectID)
        {
            return dynamic_cast<TObject*>(FindObjectOfObjectID(objectID));
        }

        ID GetObjectID() const;
        const std::string& GetName() const;
        bool GetIsAwaked() const;
        bool GetIsDestroyed() const;

        void SetName(const std::string& name);

        Object();
        ~Object() override;
        void Transfer(Transferer& transferer) override;

    protected:
        virtual void Awake(); //当前物体需要唤醒，代替构造函数
        virtual void Destroy(); //当前物体需要销毁，代替析构函数
        //virtual void Replace(const ObjectPtrBase& oldObject); //默认替换方式是直接销毁旧物体并触发其回调，可能需要依情况覆盖
        virtual void Start(); //关联物体都已唤醒，可以安全使用了
        //利用Transfer我可以知道实例化一个物体时需要连带实例化的其他物体，所以Start可以实现，但我无法知道删除一个物体需要连带删除哪些，所以没有PreDestroy

    private:
        inline static ID objectIDCount = 0;
        inline static std::map<ID, Object*> allObjects = {}; //内存中的所有Object，包括未激活的Object

        ID objectID = 0; //0为空占位符,只有原生状态会使用
        //可以解决重复调用或循环调用的问题，是激活和销毁能以安全正确的顺序执行
        bool isAwaked = false;
        bool isDestroyed = false;

        std::string name;
    };

    CustomReflection(Object)
}
