#pragma once
#include <BDXKEngine/Engine.h>

namespace BDXKEditor
{
    using namespace BDXKEngine;

    namespace CreationMenu
    {
        class Object3D
        {
        public:
            static ObjectPtr<GameObject> Cube(const char* name = "Cube", Color color = Color::white)
            {
                return CreateObject3D(Resources::GetCubeMesh(), name, color);
            }

            static ObjectPtr<GameObject> Plane(const char* name = "Plane", Color color = Color::white)
            {
                ObjectPtr<Transform> plane = Cube(name, color)->GetTransform();
                plane->SetLocalScale({10, 0.01f, 10});
                plane->SetLocalPosition({0, -0.005f, 0});
                return plane->GetGameObject();
            }

            static ObjectPtr<GameObject> Sphere(const char* name = "Sphere", Color color = Color::white)
            {
                return CreateObject3D(Resources::GetSphereMesh(), name, color);
            }

            static ObjectPtr<GameObject> CreateObject3D(ObjectPtr<Mesh> mesh, const char* name,
                                                        Color color = Color::white)
            {
                //加载着色器
                ObjectPtr<Shader> baseShader = Shader::Create(
                    GetResourcesPathW(Shaders, Standard\\VertexShader.hlsl),
                    GetResourcesPathW(Shaders, Standard\\PixelShader.hlsl),
                    PassType::ForwardBase
                );
                ObjectPtr<Shader> addShader = Shader::Create(
                    GetResourcesPathW(Shaders, Standard\\VertexShader.hlsl),
                    GetResourcesPathW(Shaders, Standard\\PixelShader.hlsl),
                    PassType::ForwardAdd
                );
                addShader->SetBlend(Blend::Additive);

                //创建纹理
                ObjectPtr<Texture2D> texture2d = Texture2D::Create(color);
                //创建材质球
                ObjectPtr<Material> material = Material::Create({baseShader, addShader});
                material->SetTexture(0, texture2d.ToObjectPtr<Texture>());
                //创建物体
                ObjectPtr<GameObject> gameObject = GameObject::Create(name);
                ObjectPtr<MeshRenderer> meshRenderer = gameObject->AddComponent<MeshRenderer>();
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
                ObjectPtr<GameObject> lightObj = GameObject::Create(name);
                ObjectPtr<BDXKEngine::Light> light = lightObj->AddComponent<BDXKEngine::Light>();
                light->SetColor(Color::white);
                light->SetIntensity(0.5f);
                light->SetLightType(lightType);
                return lightObj;
            }
        };

        static ObjectPtr<GameObject> Camera(const char* name = "Camera")
        {
            ObjectPtr<GameObject> camera = GameObject::Create(name);
            camera->AddComponent<BDXKEngine::Camera>();
            return camera;
        }

        static void CreateDefaultScene()
        {
            ObjectPtr<GameObject> sun = CreationMenu::Light::DirectionalLight("太阳");
            ObjectPtr<GameObject> ground = CreationMenu::Object3D::Plane("地面");
            ObjectPtr<GameObject> sphere = CreationMenu::Object3D::Sphere("球");
            const ObjectPtr<GameObject> camera = CreationMenu::Camera("摄像机");
            //太阳
            {
                ObjectPtr<Transform> transform = sun->GetTransform();
                transform->SetLocalEulerAngles({45, -45, 0});
            }
            //球的展示位置
            {
                ObjectPtr<Transform> transform = sphere->GetTransform();
                transform->SetLocalPosition({0, 0.5f, 0});
            }
            //增加控制功能并调整位置
            {
                camera->GetTransform()->SetLocalPosition({0, 1, -10});
            }

            // ObjectPtr<GameObject> blender = CreationMenu::Object3D::CreateObject3D(Resources::GetBlenderMesh(), "Blender");
            // blender->GetTransform()->SetLocalPosition({0, 3.0f, 0});
        }
    }
}
