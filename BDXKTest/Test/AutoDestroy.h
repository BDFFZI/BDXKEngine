﻿#pragma once
#include <BDXKEngine.h>

namespace Assembly
{
    using namespace BDXKEngine;

    class AutoDestroy : public Behavior, public UpdateHandler, public AwakeHandler, public DestroyHandler
    {
        float time = 0;
        ObjectPtr<Animator> animator;
        ObjectPtr<MeshRenderer> meshRenderer;

        void OnAwake() override
        {
            meshRenderer = GetGameObject()->GetComponent<MeshRenderer>();
            animator = GetGameObject()->AddComponent<Animator>();
            animator->SetAnimation([](const ObjectPtr<Transform> transform)
            {
                transform->SetLocalPosition(
                    transform->GetLocalPosition() + transform->GetFront() * Time::GetDeltaTime() * 3);
            });
        }

        void OnUpdate() override
        {
            time += Time::GetDeltaTime();
            animator->SetEnabling(static_cast<int>(std::fmod(time * 2.5f, 2)) == 1);
            //meshRenderer->SetEnabling(!animator->GetIsEnabling());
            if (time > 4)
            {
                ObjectPtr<GameObject> gameObject = GetGameObject();
                DestroyImmediate(this);
                DestroyImmediate(gameObject.ToObject());
                return;
            }
            if (static_cast<int>(std::fmodf(Time::GetFrameCount() - time, 100)) == 0)
            {
                Debug::Log(std::to_wstring(GetInstanceID()) + L" 随机值:" + Random::ColorHSV().ToString());
                Color color = Random::ColorHSV();
                color.a = 0.3f;

                meshRenderer->GetMaterial()->SetTexture(0, Texture2D::Create(color).ToObjectPtr<Texture>());
            }
        }

        void OnDestroy() override
        {
            Debug::Log(L"##################################");
        }
    };
}