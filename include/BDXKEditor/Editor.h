#pragma once
#include <BDXKEngine/Engine.h>
#include <BDXKEngine/Platform/IMGUI/IMGUIManager.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "BDXKEngine/Base/Serialization/Json/JsonWriter.h"
#include "Menu/CreationMenu.h"
#include "System/EditorSystem.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

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
            Engine::Run([]
            {
                // std::string name = "DBXKEngine";
                // int time = 2022;
                // Vector3 vector3 = Vector3::front;
                //
                // rapidjson::Document document;
                // rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
                // document.Parse("{}");
                //
                // document.AddMember("name", name, allocator);
                // document.AddMember("time", time, allocator);
                // document.AddMember("time", 0, allocator);
                // rapidjson::Value& array = document[time].SetArray();
                // array.PushBack(vector3.x, allocator);
                // array.PushBack(vector3.y, allocator);
                // array.PushBack(vector3.z, allocator);
                //
                // document.AddMember("vector3", array, allocator);
                //
                // rapidjson::Value nameItem(rapidjson::kStringType);
                // nameItem.SetString(name.c_str(), name.size());
                // document.PushBack(nameItem, allocator);
                //
                // rapidjson::Value intItem(rapidjson::kStringType);
                // intItem.SetInt(3);
                // document.PushBack(intItem, allocator);

                // document.AddMember("name",rapidjson::Document::SetInt(time),document.GetAllocator());
                // document.AddMember("name", rapidjson::Document::SetString(true),document.GetAllocator());

                // rapidjson::StringBuffer buffer;
                // rapidjson::Writer writer(buffer);
                // document.Accept(writer);
                // Debug::Log(buffer.GetString());
                // return;
                // JsonWriter writer{document};
                //
                // writer.TransferField("名称", name);
                // writer.TransferField("时间", time);
                // writer.TransferField("向量", vector3);


                Object::AddSerializationInfo<EditorSystem>();
                GameObject::Create("EditorSystem")->AddComponent<EditorSystem>();

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
            ObjectPtr<GameObject> aureole = GameObject::Create("光环");
            {
                ObjectPtr<Transform> transform = aureole->GetTransform();
                transform->SetParent(GameObject::Find("摄像机")->GetTransform());
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
