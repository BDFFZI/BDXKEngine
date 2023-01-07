#pragma once
#include <map>
#include <unordered_map>
#include "BDXKEngine/Base/Reflection/Reflection.h"
#include "BDXKEngine/Base/Reflection/Reflective.h"

namespace BDXKEngine
{
    template <typename TKey, typename TValue>
    class Map : public Reflective
    {
    public:
        Map()
        {
        }
        Map(const std::unordered_map<TKey, TValue>& map)
        {
            for (auto& item : map)
            {
                keys.emplace_back(item.first);
                values.emplace_back(item.second);
            }
        }
        Map(const std::map<TKey, TValue>& map)
        {
            for (auto& item : map)
            {
                keys.emplace_back(item.first);
                values.emplace_back(item.second);
            }
        }

        std::unordered_map<TKey, TValue> ToUnorderedMap()
        {
            std::unordered_map<TKey, TValue> result = {};
            const size_t count = keys.size();
            for (size_t i = 0; i < count; i++)
            {
                result[keys[i]] = values[i];
            }
            return result;
        }
        std::map<TKey, TValue> ToMap()
        {
            std::map<TKey, TValue> result = {};
            const size_t count = keys.size();
            for (size_t i = 0; i < count; i++)
            {
                result[keys[i]] = values[i];
            }
            return result;
        }
    private:
        static void StaticConstructor()
        {
            Reflection::SetReflection<Map<TKey, TValue>>();
        }
        CustomStaticConstructor(StaticConstructor)

        std::vector<TKey> keys;
        std::vector<TValue> values;

        void Transfer(Transferer& transferer) override
        {
            Reflective::Transfer(transferer);

            TransferFieldInfo(keys);
            TransferFieldInfo(values);
        }
    };
}
