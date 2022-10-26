#pragma once
#include <BDXKEngine/Engine.h>
#include <BDXKEngine/Platform/IMGUI/IMGUIManager.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "BDXKEngine/Base/Serialization/Json/JsonReader.h"
#include "BDXKEngine/Base/Serialization/Json/JsonWriter.h"
#include "Menu/CreationMenu.h"
#include "System/EditorSystem.h"


namespace BDXKEditor
{
    using namespace BDXKEngine;

    struct SerializeTester : ISerializable
    {
        void Transfer(Transferrer& transferrer) override
        {
            transferrer.TransferField("floatValue", floatValue);
            transferrer.TransferField("stringValue", stringValue);
            transferrer.TransferFieldOf<Vector4>("colorValue", colorValue);
            transferrer.TransferField("boolValue", boolValue);
        }

        float floatValue;
        std::string stringValue;
        Color colorValue;
        bool boolValue;
    };

    class Editor
    {
    public:
        static bool GetObjectGUID(int instanceID, GUID& guid)
        {
            const auto result = objectGUIDs.find(instanceID);
            if (result != objectGUIDs.end())
            {
                guid = result->second;
                return true;
            }

            return false;
        }

        static void Run()
        {
            Engine::Run([](const ObjectPtr<Scene>& scene)
            {
                EditorSystem::mainScene = scene;
                
                Object::AddSerializationInfo<EditorSystem>();
                scene->AddGameObject("EditorSystem")->AddComponent<EditorSystem>();
                
                CreationMenu::CreateDefaultScene();
                TestLight();
            });

            Debug::Log("\n按任意键退出");
            std::getchar();
        }

    private:
        inline static Window* hierarchyView;
        inline static IMGUIManager* hierarchyViewImgui;
        inline static Window* inspectorView;
        inline static std::map<int, GUID> objectGUIDs;
        inline static std::map<GUID, std::string> objectAssets;
        inline static ObjectPtr<GameObject> editorSystem;


        static void TestLight()
        {
            ObjectPtr<GameObject> aureole = EditorSystem::mainScene->AddGameObject("光环");
            {
                ObjectPtr<Transform> transform = aureole->GetTransform();
                transform->SetParent(EditorSystem::mainScene->Find("摄像机")->GetTransform());
                transform->SetLocalPosition({0, 0, 0.7f});

                ObjectPtr<Animator> animator = aureole->AddComponent<Animator>();
                animator->SetAnimation(AnimatorClip::Create([](ObjectPtr<Transform> transform)
                {
                    Vector3 position = transform->GetLocalPosition();
                    position.x = std::cosf(Time::GetRealtimeSinceStartup()) * 0.3f;
                    position.y = std::sinf(Time::GetRealtimeSinceStartup()) * 0.3f;
                    transform->SetLocalPosition(position);

                    Vector3 eulerAngles = transform->GetLocalEulerAngles();
                    eulerAngles.z -= 90 * Time::GetDeltaTime();
                    transform->SetLocalEulerAngles(eulerAngles);
                }));
            }

            ObjectPtr<GameObject> sphere = CreationMenu::Object3D::Sphere("小球");
            ObjectPtr<Light> sphere_light;
            {
                ObjectPtr<Transform> transform = sphere->GetTransform();
                transform->SetParent(aureole->GetTransform());
                transform->SetLocalScale({0.1f, 0.1f, 0.1f});

                sphere_light = CreationMenu::Light::PointLight("红色点光源")->GetComponent<Light>();
                sphere_light->SetLightType(LightType::Point);
                sphere_light->SetColor(Color::red);
                sphere_light->SetIntensity(0.5f);
                sphere_light->GetTransform()->SetParent(sphere->GetTransform());
            }

            ObjectPtr<GameObject> cube = CreationMenu::Object3D::Cube("棍子");
            {
                ObjectPtr<Transform> transform = cube->GetTransform();
                transform->SetParent(aureole->GetTransform());
                transform->SetLocalScale({0.2f, 0.05f, 0.05f});
            }
        }
    };
}
