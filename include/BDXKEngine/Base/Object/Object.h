﻿#pragma once
#include <functional>
#include <vector>
#include <map>
#include "BDXKEngine/Base/Extension/String.h"
#include "BDXKEngine/Base/Serialization/ISerializable.h"
#include "BDXKEngine/Base/Serialization/Binary/BinaryImporter.h"
#include "BDXKEngine/Base/Serialization/Binary/BinaryExporter.h"
#include "BDXKEngine/Base/Extension/Debug.h"


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

namespace BDXKEngine
{
    class ObjectManager;

    class Object : public ISerializable
    {
        friend ObjectManager;
    public:
        static void DebugObjectCount();

        template <typename TObject>
        static std::string ParseSerializationID()
        {
            return std::string(typeid(TObject).name()).substr(6);
        }
        static std::string ParseSerializationID(Object* object)
        {
            return std::string(typeid(*object).name()).substr(6);
        }
        //希望物体能参与序列化则必须要注册SerializationInfo
        template <typename TObject>
        static void AddSerializationInfo()
        {
            serializationInfo.insert({ParseSerializationID<TObject>().c_str(), [] { return Object::Create<TObject>(); }});
        }

        // 代替new函数
        // 使用Create()创建的Object的InstanceID不可能等于0
        // 以此区分是否为合法的Object
        template <typename TObject>
        static TObject* Create()
        {
            auto* result = new TObject();
            result->instanceID = ++instanceIDCount;
            result->name = typeid(TObject).name();
            allObjects[result->instanceID] = result;
            //Debug::LogWarning(std::string("Object::Create ") + std::to_string(result->instanceID) + " " + result->GetName());
            return result;
        }


        // 初始物体，仅加载序列化层面功能，不进行实例化，为无运行时功能的静态物体
        template <typename TObject>
        static TObject* InstantiateNoAwake(Transferrer& importer)
        {
            Object* result = Create<TObject>();
            result->Transfer(importer);
            return static_cast<TObject*>(result);
        }

        static Object* InstantiateNoAwake(Object* serializer);

        template <typename TObject>
        static TObject* InstantiateNoAwake(TObject* serializer)
        {
            return static_cast<TObject*>(InstantiateNoAwake(static_cast<Object*>(serializer)));
        }
        /// 实例化物体，以加载运行时相关功能
        template <typename TObject>
        static TObject* Instantiate(TObject* source)
        {
            if (source == nullptr)
                throw std::exception("用于实例化的参照物体为空");

            TObject* object = InstantiateNoAwake<TObject>(source);
            if (source->instanceID != 0)object->SetName(object->GetName() + " (Clone)");

            const std::vector<Object*> lastAwakeQueue = postAwakeQueue;
            postAwakeQueue.clear();
            Awake(object);
            FlushAwakeQueue();
            postAwakeQueue = lastAwakeQueue;

            return object;
        }

        /// 卸载物体，从内存层面真正卸载
        static void DestroyImmediate(Object* object);

        template <typename TObject>
        static std::vector<TObject*> FindObjectsOfType()
        {
            std::vector<TObject*> result{};
            for (auto& item : allObjectsRunning)
            {
                TObject* object = dynamic_cast<TObject*>(item.second);
                if (object != nullptr)result.push_back(object);
            }

            return result;
        }

        static Object* FindObjectOfInstanceID(int instanceID);

        int GetInstanceID() const;
        std::string GetName();
        bool GetIsRunning() const;

        void SetName(const std::string& name);

        virtual std::string ToString();
    protected:
        /// 代替构造函数，激活物体，只在Awake()中调用有效，类似一直标记信号的传递
        static void Awake(Object* object);
        /// 代替析构函数，销毁物体，只在Destroy()中调用有效，单独抽出为静态函数是为了解决递归调用的问题,可以保证每次删除都充分得到检查和修正，类似一直标记信号的传递
        static void Destroy(Object* object);


        void Transfer(Transferrer& transferrer) override;
        virtual void Awake(); //唤醒回调，用于扩展Awake(Object* object)函数的内容
        virtual void Destroy(); //销毁回调，用于扩展只在Destroy(Object* object)函数的内容
    private:
        static std::unordered_map<std::string, std::function<Object*()>> serializationInfo;
        static int instanceIDCount; //0为None占位符,一般用作纯数据容器
        /// 内存中的所有Object，包括未实例化的Object
        static std::unordered_map<int, Object*> allObjects;
        static std::unordered_map<int, Object*> allObjectsRunning;
        static std::vector<Object*> postAwakeQueue;
        static std::vector<Object*> postDestroyQueue;

        // 统一销毁被标记的物体，DestroyImmediate()调用，预留Unity延迟删除功能，应该是为了内存回收的问题
        static void FlushAwakeQueue();
        static void FlushDestroyQueue();

        int instanceID = 0;
        std::string name;
        bool isRunning = false;
        bool isDestroying = false;
    };
}