#pragma once
#include "BDXKEngine/Function/Graphics/GraphicsInfo.h"
#include "BDXKEngine/Platform/GL/Mesh/Mesh.h"
#include "Core/GameObject.h"
#include "Renderer/Core/RenderSettings.h"

namespace BDXKEngine
{
    namespace Creation
    {
        ObjectPtr<GameObject> CreateCube(const char* name = "Cube", Color color = Color::white);
        ObjectPtr<GameObject> CreatePlane(const char* name = "Plane", Color color = Color::white);
        ObjectPtr<GameObject> CreateSphere(const char* name = "Sphere", Color color = Color::white);
        ObjectPtr<GameObject> CreateObject3D(const ObjectPtr<Mesh>& mesh, const char* name, Color color = Color::white);
        ObjectPtr<GameObject> CreateDirectionalLight(const char* name = "DirectionalLight");
        ObjectPtr<GameObject> CreatePointLight(const char* name = "PointLight");
        ObjectPtr<GameObject> CreateLight(LightType lightType, const char* name = "Light");
        ObjectPtr<GameObject> CreateCamera(const char* name = "Camera");
    }

    class Scene : public Object
    {
    public:
        static void LoadDefault();
        static void Load(const ObjectPtr<Scene>& path);
        static ObjectPtr<Scene> GetCurrentScene();

        std::vector<ObjectPtr<GameObject>>& GameObjects();
    private:
        ObjectPtr<RenderSettings> renderSettings;
        std::vector<ObjectPtr<GameObject>> gameObjects;
        void Transfer(Transferer& transferer) override;
    };

    CustomReflection(Scene)
}
