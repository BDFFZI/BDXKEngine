#pragma once
#include "BDXKEngine/Base/Object/ObjectPtr.h"
#include "BDXKEngine/Framework/Object/SwitchableObject.h"
#include "GraphicsSettings.h"
#include "QualitySettings.h"

namespace BDXKEngine
{
    class GameObject;

    class Scene : public SwitchableObject, ObjectManager
    {
        friend GameObject;
    public:
        static ObjectPtr<Scene> Create(); 
        
        const ObjectPtr<GraphicsSettings>& GetGraphicsSettings() const;
        const ObjectPtr<QualitySettings>& GetQualitySettings() const;

        ObjectPtr<GameObject> AddGameObject(const std::string& name = "New GameObject");
        ObjectPtr<GameObject> Find(const std::string& name);
    private:
        //所有物体(由GameObject负责增减)
        std::vector<ObjectPtr<GameObject>> gameObjects;
        ObjectPtr<GraphicsSettings> graphicsSettings;
        ObjectPtr<QualitySettings> qualitySettings;

        void Awake() override;
        void Destroy() override;
    };
}
