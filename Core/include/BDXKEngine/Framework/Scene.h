#pragma once
#include "BDXKEngine/Function/Graphics/GraphicsInfo.h"
#include "BDXKEngine/Platform/GL/Mesh/Mesh.h"
#include "Core/GameObject.h"
#include "Renderer/Core/RenderSettings.h"

namespace BDXKEngine
{
    class SceneDefault
    {
    public:
        static ObjectPtr<GameObject> CreateCube(const char* name = "Cube", Color color = Color::white);
        static ObjectPtr<GameObject> CreatePlane(const char* name = "Plane", Color color = Color::white);
        static ObjectPtr<GameObject> CreateSphere(const char* name = "Sphere", Color color = Color::white);
        static ObjectPtr<GameObject> CreateObject3D(const ObjectPtr<Mesh>& mesh, const char* name, Color color = Color::white);
        static ObjectPtr<GameObject> CreateDirectionalLight(const char* name = "DirectionalLight");
        static ObjectPtr<GameObject> CreatePointLight(const char* name = "PointLight");
        static ObjectPtr<GameObject> CreateLight(LightType lightType, const char* name = "Light");
        static ObjectPtr<GameObject> CreateCamera(const char* name = "Camera");
    };

    class Scene : public Object
    {
    public:
        static void LoadDefault();
        static void Load(const std::string& sceneName, bool keepPersistent = false);
        static void Save(const std::string& sceneName);
        static ObjectPtr<Scene> GetCurrentScene();
    private:
        ObjectPtr<RenderSettings> renderSettings;
        std::vector<ObjectPtr<GameObject>> gameObjects;
        
        void Transfer(Transferer& transferer) override;
    };

    CustomReflection(Scene)
}
