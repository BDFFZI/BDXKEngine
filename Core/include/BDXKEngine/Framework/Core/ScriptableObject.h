#pragma once
#include <unordered_set>
#include "BDXKEngine/Base/Object/Object.h"
#include "BDXKEngine/Base/Reflection/Reflection.h"

namespace BDXKEngine
{
    class ScriptableObject;

    class AwakeHandler
    {
        friend ScriptableObject;
    public:
        virtual ~AwakeHandler() = default;
    protected:
        virtual void OnAwake() = 0;
    };

    class DestroyHandler
    {
        friend ScriptableObject;
    public:
        virtual ~DestroyHandler() = default;
    protected:
        virtual void OnDestroy() = 0;
    };

    class EnableHandler
    {
        friend ScriptableObject;
    public:
        virtual ~EnableHandler() = default;
    protected:
        virtual void OnEnable() = 0;
    };

    class DisableHandler
    {
        friend ScriptableObject;
    public:
        virtual ~DisableHandler() = default;
    protected:
        virtual void OnDisable() = 0;
    };

    //脚本能力有两层含义，1.提供基本的脚本事件，2.提供被其他脚本搜索的功能
    class ScriptableObject : public Object
    {
    public:
        template <class TTarget, class TBack = TTarget>
        static std::vector<ObjectPtr<TBack>> FindScriptableObjectsOfType()
        {
            std::vector<ObjectPtr<TBack>> result{};
            for (const auto& item : allScriptableObjects)
            {
                TTarget* object = dynamic_cast<TTarget*>(item);
                if (object != nullptr)result.emplace_back(dynamic_cast<TBack*>(object));
            }

            return result;
        }

        virtual bool GetIsActivating() const;
        bool GetIsEnabling() const;
        void SetIsActivating(bool state);
        void SetIsEnabling(bool state);

        bool IsNotResource() const;
        bool IsActivatingAndEnabling() const;

        void Transfer(Transferer& transferer) override;
    protected:
        virtual void Enable();
        virtual void Disable();
        void Awake() override;
        void PostAwake() override;
        void PreDestroy() override;
    private:
        static std::unordered_set<ScriptableObject*> allScriptableObjects;
        
        bool isActivating = true;
        bool isEnabling = true;
        bool isAwakened = false;
    };

    CustomReflection(ScriptableObject)
}
