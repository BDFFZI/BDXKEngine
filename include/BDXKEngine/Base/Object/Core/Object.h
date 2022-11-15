#pragma once
#include <vector>
#include "BDXKEngine/Base/Serialization/Serializer.h"
#include "ObjectPtr.h"


namespace BDXKEngine
{
    class ObjectManager;

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
        friend ObjectManager;
    public:
        static std::map<int, Object*> GetAllObjects();
        //转换
        static ObjectPtrBase InstantiateNoAwake(Object* object); //将object从原生状态转为实例状态
        static ObjectPtrBase Instantiate(Object* object); //将object从原生状态转为实例状态并激活
        template <typename TObject>
        static ObjectPtr<TObject> InstantiateNoAwake(TObject* object) //将object从原生状态转为实例状态
        {
            return InstantiateNoAwake(static_cast<Object*>(object)).ToObject<TObject>();
        }
        template <typename TObject>
        static ObjectPtr<TObject> Instantiate(TObject* object) //将object从原生状态转为实例状态
        {
            return Instantiate(static_cast<Object*>(object)).ToObject<TObject>();
        }
        //克隆
        static ObjectPtrBase InstantiateNoAwake(const ObjectPtrBase& objectPtr, Serializer& serializer);
        static ObjectPtrBase Instantiate(const ObjectPtrBase& objectPtr, Serializer& serializer);
        template <typename TObject>
        static ObjectPtr<TObject> InstantiateNoAwake(const ObjectPtr<TObject>& objectPtr, Serializer& serializer)
        {
            return InstantiateNoAwake(static_cast<ObjectPtrBase>(objectPtr), serializer).template ToObject<TObject>();
        }
        template <typename TObject>
        static ObjectPtr<TObject> Instantiate(const ObjectPtr<TObject>& objectPtr, Serializer& serializer)
        {
            return Instantiate(static_cast<ObjectPtrBase>(objectPtr), serializer).template ToObject<TObject>();
        }


        //销毁物体
        static void DestroyImmediate(const ObjectPtrBase& object);

        template <typename TObject>
        static std::vector<ObjectPtr<TObject>> FindObjectsOfType()
        {
            std::vector<ObjectPtr<TObject>> result{};
            for (auto& item : allObjectsRunning)
            {
                TObject* object = dynamic_cast<TObject*>(item.second);
                if (object != nullptr)result.emplace_back(object);
            }

            return result;
        }
        static Object* FindObjectOfInstanceID(int instanceID);

        int GetInstanceID() const;
        std::string GetName() const;
        void SetName(const std::string& name);
        bool IsRunning() const; //是否是已被Awake的状态

        void Transfer(Transferer& transferer) override;

        virtual std::string ToString();
    protected:
        //单独抽出为静态函数是为了解决递归调用的问题,可以保证每次删除都充分得到检查和修正，比如防止重复唤醒销毁
        /// 用于唤醒信号的传递，调用源头为Instantiate(TObject*)，只在PreAwake()中调用有效
        static void MarkAwake(const ObjectPtrBase& objectPtr);
        /// 用于销毁信号的传递，调用源头为DestroyImmediate(TObject*)，只在PreDestroy()中调用有效
        static void MarkDestroy(const ObjectPtrBase& objectPtr);

        //以下函数以及其他同类型函数重写时请务必回调父类函数
        virtual void MarkAwake(); //预唤醒，用于传递PreAwake(Object* object)函数的范围
        virtual void MarkDestroy(); //预销毁，用于传递PreDestroy(Object* object)函数的范围，此时所影响物体都任未删除
        virtual void Awake(); //此时所影响物体都已确实唤醒
        virtual void Destroy(); //代替析构函数，销毁物体
    private:
        static int instanceIDCount; //0为None占位符,一般用作纯数据容器
        /// 内存中的所有Object，包括未实例化的Object
        static std::map<int, Object*> allObjects;
        static std::map<int, Object*> allObjectsRunning;
        static std::vector<Object*> postAwakeQueue;
        static std::vector<Object*> postDestroyQueue;

        // 统一销毁被标记的物体，DestroyImmediate()调用，预留Unity延迟删除功能，应该是为了内存回收的问题
        static void FlushAwakeQueue();
        static void FlushDestroyQueue();

        std::string name;
        int instanceID = 0; //0是默认值，作为空值使用
        bool isRunning = false;
        bool isDestroying = false;
    };
}
