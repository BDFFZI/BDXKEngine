#pragma once
#include <string>

#include "Core/Editor/AssetImporter/ObjectImporter.h"
#include "Core/Editor/Core/Assets/AssetDatabase.h"
#include "Core/Runtime/Base/Object/Pointer/ObjectPtr.h"
#include "Core/Runtime/Framework/Scene/Scene.h"

namespace BDXKEditor
{
    using namespace BDXKEngine;

    //将Setting设为静态类型，在Transfer中传输静态字段，这样静态类也可序列化
    class Settings
    {
    public:
        template <typename TSetting>
        static ObjectPtr<TSetting> SetEditorSetting()
        {
            const Type type = GetTypeOf<TSetting>();
            const std::filesystem::path settingPath = ParsePath(GetTypeName(type)).concat(".setting");
            ObjectPtr<TSetting> setting;

            if (exists(settingPath))
                setting = AssetDatabase::Load<TSetting>(settingPath);
            else
            {
                setting = Object::Create<TSetting>();
                AssetDatabase::Save(settingPath, setting);
            }

            settings[type] = setting;
            return setting;
        } //Start>-10
        template <typename TSetting>
        static ObjectPtr<TSetting> SetRuntimeSetting()
        {
            ObjectPtr<TSetting> setting = SetEditorSetting<TSetting>();

            Scene::AddLoadSceneEvent([=](const ObjectPtr<Scene>& scene)
            {
                if (scene->HaveObject(setting) == false)
                    scene->AddObject(setting, true);
            });

            return setting;
        } //Start>-10
        template <typename TSetting>
        static ObjectPtr<TSetting> GetSetting()
        {
            return settings[GetTypeOf<TSetting>()].template ToObject<TSetting>();
        }

        static std::filesystem::path ParsePath(const std::string& fileName);

    private:
        inline static std::unordered_map<Type, ObjectPtrBase> settings;

        static void OnEngineStart();
        static void OnEngineDestory();
        CustomEngineStartEvent(OnEngineStart, -20)
        CustomEngineDestoryEvent(OnEngineDestory, 10)
    };

    CustomAssetsImporter(setting, ObjectImporter)

    //     struct EditorSettingRegister
    //     {
    //         template <typename TSetting>
    //         EditorSettingRegister()
    //         {
    //             Settings::SetEditorSetting<TSetting>();
    //         }
    //     };
    //     struct RuntimeSettingRegister
    //     {
    //         template <typename TSetting>
    //         RuntimeSettingRegister()
    //         {
    //             Settings::SetRuntimeSetting<TSetting>();
    //         }
    //     };
    //
    // #define CustomEditorSetting(type)
    // #define CustomRuntimeSetting(type)
}
