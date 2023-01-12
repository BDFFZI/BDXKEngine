#pragma once
#include <map>
#include <ranges>
#include <vector>
#include "Pointer/ObjectPtr.h"

namespace BDXKEngine
{
    typedef unsigned int ID;

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
        static const std::map<ID, Object*>& GetObjects();
        static void ReplaceObject(ObjectPtrBase& source, ID targetInstanceID);
        static void SetConstructedObjectEvent(const std::function<void(Object*)>& onConstructedObject);
        static ObjectPtrBase Create(const Type& type);
        template <class TObject>
        static ObjectPtr<TObject> Create()
        {
            ObjectPtr window = new TObject{};
            Instantiate(window);
            return window;
        }

        Object();
        ~Object() override;

        //转换，将object从原生状态转为实例状态，使用序列化功能创建的物体默认都是原生状态
        static void Instantiate(ObjectPtrBase& objectPtr);
        //销毁
        static void DestroyImmediate(const ObjectPtrBase& object);
        //搜索
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
        static Object* FindObjectOfInstanceID(ID instanceID);
        template <typename TObject>
        static TObject* FindObjectOfInstanceID(ID instanceID)
        {
            return dynamic_cast<TObject*>(FindObjectOfInstanceID(instanceID));
        }

        ID GetInstanceID() const;

        bool IsInstantiated() const;
        bool IsDestroyed() const;
    protected:
        virtual void Awake(); //代替构造函数
        virtual void Destroy(); //代替析构函数
        virtual void PostAwake()
        {
        }
        virtual void PreDestroy()
        {
        }
    private:
        inline static ID instanceIDCount = 0;
        inline static std::map<ID, Object*> allObjects = {}; //内存中的所有Object，包括未激活的Object
        inline static std::function<void(Object*)> onConstructedObject = {};
        
        ID instanceID = 0; //0为空占位符,只有原生状态会使用
        //可以解决重复调用或循环调用的问题，是激活和销毁能以安全正确的顺序执行
        bool isInstantiated = false;
        bool isDestroyed = false;
    };

    CustomReflection(Object)
}
