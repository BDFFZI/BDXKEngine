#include "Object.h"
#include "Serialization/ObjectRef.h"
#include "Transferer/ObjectPtrTransferer.h"

namespace BDXKEngine
{
    const std::map<ID, Object*>& Object::GetAllObjects()
    {
        return allObjects;
    }
    void Object::ReplaceObject(ObjectPtrBase& newObject, const ID targetObjectID)
    {
        Object* object = newObject.ToObject();
        if (object == nullptr)
            throw std::exception("替换物体不存在");
        if (object->GetObjectID() == targetObjectID)
            throw std::exception("新旧ID相同");
        if (targetObjectID <= 0)
            throw std::exception("目标ID不可使用");

        const Object* oldObject = FindObjectOfObjectID(targetObjectID);
        DestroyObject(oldObject);
        delete oldObject;

        allObjects[targetObjectID] = object;
        allObjects.erase(object->objectID);
        object->objectID = targetObjectID;

        newObject = {object}; //source.objectID已从allObjects移除，所以替换时并不会导致错误删除
    }

    ObjectPtrBase Object::Create(const Type& type)
    {
        const auto object = dynamic_cast<Object*>(Reflection::GetReflection(type).GetConstruction());
        if (object == nullptr)throw std::exception("目标未Object类型");
        ObjectPtrBase window = object;
        AwakeObject(window);
        return window;
    }

    void Object::AwakeObject(const ObjectPtrBase& objectPtr)
    {
        if (objectPtr.IsNull()) throw std::exception("实例化的物体为空");

        //获取关联物体
        ObjectPtrTransferer referenceTransferer = {};
        referenceTransferer.TransferObjectPtr(const_cast<ObjectPtrBase&>(objectPtr));
        objectPtr->Transfer(referenceTransferer);
        auto references = referenceTransferer.GetObjectIDs();

        //唤醒所有关联物体
        std::vector<ID> awakeObjects;
        for (auto reference = references.begin(); reference != references.end(); ++reference)
        {
            Object* referenceObject = FindObjectOfObjectID(*reference);
            if (referenceObject != nullptr && referenceObject->isAwaked == false)
            {
                referenceObject->isAwaked = true;
                referenceObject->Awake();
                awakeObjects.push_back(*reference);
            }
        }

        //所有唤醒已完成
        for (const auto objectID : awakeObjects)
        {
            Object* referenceObject = FindObjectOfObjectID(objectID);
            if (referenceObject != nullptr)referenceObject->Start();
        }
    }
    void Object::DestroyObject(const ObjectPtrBase& objectPtr)
    {
        Object* object = objectPtr.ToObject();
        if (object == nullptr || object->isDestroyed)
            return;

        if (object->isAwaked == false) //根本就没被唤醒过，所以也不需要调用销毁回调
        {
            delete object;
            return;
        }

        object->isDestroyed = true;
        object->Destroy();
        delete object;
    }

    Object* Object::FindObjectOfObjectID(const ID objectID)
    {
        if (objectID == 0)
            return nullptr;
        if (const auto pair = allObjects.find(objectID); pair != allObjects.end())
            return pair->second;

        return nullptr;
    }

    ID Object::GetObjectID() const
    {
        return objectID;
    }
    const std::string& Object::GetName() const
    {
        return name;
    }
    bool Object::GetIsAwaked() const
    {
        return isAwaked;
    }
    bool Object::GetIsDestroyed() const
    {
        return isDestroyed;
    }

    void Object::SetName(const std::string& name)
    {
        this->name = name;
    }

    Object::Object()
    {
        //运行时引用
        objectID = ++objectIDCount;
        allObjects[objectID] = this;
    }
    Object::~Object()
    {
        allObjects.erase(objectID);
    }
    void Object::Transfer(Transferer& transferer)
    {
        Reflective::Transfer(transferer);
        TransferFieldInfo(name);
    }

    void Object::Awake()
    {
        if (name.empty())
            name = GetTypeName(GetType());

        //std::cout << "Object::Awake " + std::to_string(objectID) + " " + name << std::endl;
    }
    void Object::Start()
    {
    }
    void Object::Destroy()
    {
        //std::cout << "Object::Destroy " + std::to_string(objectID) + " " + name << std::endl;
    }
}
