#pragma once
#include <crossguid/guid.hpp>
#include "Core/Runtime/Base/Object/Object.h"

namespace BDXKEngine
{
    class Scene : public Object
    {
    public:
        static void AddLoadSceneEvent(const std::function<void(const ObjectPtr<Scene>&)>& event);
        static ObjectPtr<Scene> GetActiveScene();
        static ObjectPtr<Scene> GetDurableScene(); //该场景的物体在加载新场景时不会删除,且该场景不会触发加载回调

        static bool IsExisting(const std::string& sceneName);
        static void Create(const std::string& sceneName, const xg::Guid& persistent = {});
        static void Load(const std::string& name, const xg::Guid& persistent = {});
        static void UnLoad();
        static void Save(const ObjectPtr<Scene>& scene);

        const std::vector<ObjectPtrBase>& GetAllObjects();
        void AddObject(const ObjectPtrBase& objectPtr, bool first = false);
        void RemoveObject(const ObjectPtrBase& objectPtr);
        bool HaveObject(const ObjectPtrBase& objectPtr);

    private:
        inline static ObjectPtr<Scene> activeScene;
        inline static ObjectPtr<Scene> durableScene = Object::Create<Scene>("DurableScene");
        inline static std::vector<std::function<void(const ObjectPtr<Scene>&)>> loadSceneEvents;
        static xg::Guid NameToGuid(const std::string& name);

        std::vector<ObjectPtrBase> objects;

        void Transfer(Transferer& transferer) override;
        void Destroy() override;
    };

    CustomReflection(Scene)
}
