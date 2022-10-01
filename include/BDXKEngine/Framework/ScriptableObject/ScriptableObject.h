#pragma once
#include <unordered_set>
#include "BDXKEngine/Framework/SwitchableObject.h"

namespace BDXKEngine
{
    class ScriptableObject : public SwitchableObject
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

        bool GetIsActivating() const override;
        void SetIsEnabling(bool state) override;
    protected:
        static std::unordered_set<ScriptableObject*> allScriptableObjects;
        bool isAwakened = false;

        virtual void Enable();
        virtual void Disable();
        void Destroy() override;

        void OnUpdateActivating(bool state) override;
    };
}
