#pragma once
#include <unordered_set>
#include "BDXKEngine/Base/Object/Core/Object.h"

namespace BDXKEngine
{
    //脚本能力有两层含义，1.提供基本的脚本事件，2.提供被其他脚本搜索的功能
    class ScriptableObject : public Object
    {
    public:
        template <typename TObject>
        static std::vector<TObject*> FindScriptableObjectsOfType()
        {
            std::vector<TObject*> result{};
            for (const auto& item : allScriptableObjects)
            {
                TObject* object = dynamic_cast<TObject*>(item);
                if (object != nullptr)result.push_back(object);
            }

            return result;
        }

        virtual bool GetIsActivating() const;
        bool GetIsEnabling() const;
        void SetIsActivating(bool state);
        void SetIsEnabling(bool state);
        
        bool IsActivatingAndEnabling() const;
        
        void Transfer(Transferer& transferer) override;
    protected:
        virtual void Enable();
        virtual void Disable();
        void Awake() override;
        void Destroy() override;
    private:
        static std::unordered_set<ScriptableObject*> allScriptableObjects;

        bool isAwakened = false;
        
        bool isEnabling = true;
    };
}
