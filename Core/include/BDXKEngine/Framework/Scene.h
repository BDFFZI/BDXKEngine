#pragma once
#include "BDXKEngine/Function/Resources/Resources.h"
#include "Core/GameObject.h"
#include "Renderer/Core/RenderSettings.h"

namespace BDXKEngine
{
    /**
     * \brief Scene只是一个序列化容器
     * 
     * Scene不等于当前场景。
     * Scene只是为了持久化目的而设计的包装器。
     * 只有在加载和保存场景时才会临时用到Scene。
     * 
     * 对Scene来说instanceID和Guid实际上都是无效的。
     * 因为Scene永远只是临时使用，不会被任何物体长期保存，所以并不需要Object提供的引用功能。
     * 加载保存Scene都是直接使用文件名，所以自身的Guid不会被使用。
     * 继承Object的唯一原因是为了使用Serialization提供的序列化功能。
     *
     * 当使用场景另存为功能时，新旧Scene中的Guid会出现重复，这种情况下加载的物体会被识别为同一个。
     * 相同物体被加载便会触发替换功能，但因为加载场景前都会卸载旧场景，不可能出现两个场景同时存在的情况。
     * 所以这种情况也不成问题。
     */
    class Scene : public Object
    {
    public:
        static ObjectPtr<Scene> GetCurrentScene();
        static std::string& GetCurrentSceneName();

        static void Create(const std::string& sceneName);
        static void Load(const std::string& sceneName, bool retainPersistent = false);
        static void Save();
        static void SaveAs(const std::string& sceneName);
    private:
        static std::string currentSceneName;

        ObjectPtr<RenderSettings> renderSettings;
        std::vector<ObjectPtr<GameObject>> gameObjects;

        void Transfer(Transferer& transferer) override;
    };

    CustomReflection(Scene)
}
