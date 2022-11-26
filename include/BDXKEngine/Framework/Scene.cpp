#include "Scene.h"
#include "BDXKEngine/Function/Assets/Assets.h"
#include "BDXKEngine/Platform/Resources/Resources.h"
#include "Renderer/MeshRenderer.h"
#include "Renderer/Core/Camera.h"
#include "Renderer/Core/Light.h"

namespace BDXKEngine
{
    namespace Creation
    {
        class Object3D
        {
        public:
            static ObjectPtr<GameObject> Cube(const char* name = "Cube", Color color = Color::white)
            {
                return CreateObject3D(Assets::GetCubeMesh(), name, color);
            }

            static ObjectPtr<GameObject> Plane(const char* name = "Plane", Color color = Color::white)
            {
                ObjectPtr<GameObject> plane = Cube(name, color);
                plane->SetLocalScale({10, 0.01f, 10});
                plane->SetLocalPosition({0, -0.005f, 0});
                return plane;
            }

            static ObjectPtr<GameObject> Sphere(const char* name = "Sphere", Color color = Color::white)
            {
                return CreateObject3D(Assets::GetSphereMesh(), name, color);
            }

            static ObjectPtr<GameObject> CreateObject3D(const ObjectPtr<Mesh>& mesh, const char* name,
                                                        Color color = Color::white)
            {
                //创建材质球
                const ObjectPtr<Material> material = Resources::Clone(Assets::GetStandardMaterial());
                material->SetTexture2D(0, Texture2D::Create(color));
                //创建物体
                const ObjectPtr<GameObject> gameObject = GameObject::Create(name);
                const ObjectPtr<MeshRenderer> meshRenderer = Component::Create<MeshRenderer>(gameObject);
                meshRenderer->SetMesh(mesh);
                meshRenderer->SetMaterial(material);

                return gameObject;
            }
        };

        class Light
        {
        public:
            static ObjectPtr<GameObject> DirectionalLight(const char* name = "DirectionalLight")
            {
                return CreateLight(LightType::Directional, name);
            }

            static ObjectPtr<GameObject> PointLight(const char* name = "PointLight")
            {
                return CreateLight(LightType::Point, name);
            }

        private:
            static ObjectPtr<GameObject> CreateLight(LightType lightType, const char* name = "Light")
            {
                const ObjectPtr<GameObject> gameObject = GameObject::Create(name);
                const ObjectPtr<BDXKEngine::Light> light = Component::Create<BDXKEngine::Light>(gameObject);
                light->SetColor(Color::white);
                light->SetIntensity(0.5f);
                light->SetType(lightType);
                return gameObject;
            }
        };

        static ObjectPtr<GameObject> Camera(const char* name = "Camera")
        {
            ObjectPtr<GameObject> gameObject = GameObject::Create(name);
            Component::Create<BDXKEngine::Camera>(gameObject);
            return gameObject;
        }
    }

    ObjectPtr<Scene> Scene::CreateDefault()
    {
        const ObjectPtr<GameObject> sun = Creation::Light::DirectionalLight("太阳");
        const ObjectPtr<GameObject> ground = Creation::Object3D::Plane("地面");
        const ObjectPtr<GameObject> sphere = Creation::Object3D::Sphere("球");
        const ObjectPtr<GameObject> camera = Creation::Camera("摄像机");
        sun->SetLocalEulerAngles({45, -45, 0});
        sphere->SetLocalPosition({0, 0.5f, 0});
        camera->SetLocalPosition({0, 1, -10});

        ObjectPtr scene = new Scene{};
        Instantiate(scene);

        scene->renderSettings = RenderSettings::Create();
        scene->gameObjects = {sun, ground, sphere, camera};

        return scene;
    }
    void Scene::Transfer(Transferer& transferer)
    {
        Object::Transfer(transferer);

        TransferFieldInfo(renderSettings);
        TransferFieldInfo(gameObjects);
    }
}
