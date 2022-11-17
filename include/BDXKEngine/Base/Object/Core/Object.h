#pragma once
#include <vector>
#include "BDXKEngine/Base/Serialization/Serializer.h"
#include "ObjectPtr.h"


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
        static std::map<int, Object*> GetAllObjects();

        Object();
        ~Object() override;

        //转换，将object从原生状态转为实例状态
        [[nodiscard]] static ObjectPtrBase Instantiate(const ObjectPtrBase& objectPtr);
        template <typename TObject>
        [[nodiscard]] static ObjectPtr<TObject> Instantiate(const ObjectPtr<TObject>& objectPtr)
        {
            return Instantiate(static_cast<ObjectPtrBase>(objectPtr)).ToObject<TObject>();
        }
        //克隆
        static ObjectPtrBase Instantiate(const ObjectPtrBase& objectPtr, Serializer& serializer);
        template <typename TObject>
        static ObjectPtr<TObject> Instantiate(const ObjectPtr<TObject>& objectPtr, Serializer& serializer)
        {
            return Instantiate(static_cast<ObjectPtrBase>(objectPtr), serializer).template ToObject<TObject>();
        }
        //销毁
        static void DestroyImmediate(const ObjectPtrBase& object);
        //搜索
        template <typename TObject>
        static std::vector<TObject*> FindObjectsOfType()
        {
            std::vector<TObject*> result{};
            for (auto& item : allObjects)
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

        void Transfer(Transferer& transferer) override;

        virtual std::string ToString();
    protected:
        virtual void Awake(); //代替构造函数
        virtual void Destroy(); //代替析构函数
    private:
        static int instanceIDCount;
        static std::map<int, Object*> allObjects; //内存中的所有Object，包括未激活的Object

        std::string name;
        int instanceID = 0; //0为空占位符,只有原生状态会使用
        //可以解决重复调用或循环调用的问题，是激活和销毁能以安全正确的顺序执行
        bool isRunning = false;
        bool isDestroying = false;
    };
}
