#include "Light.h"
#include <algorithm>
#include "Renderer.h"
#include "RenderSettings.h"
#include "BDXKEngine/Function/Graphics/Graphics.h"

namespace BDXKEngine
{
    std::vector<Light*> Light::lights = {};

    const std::vector<Light*>& Light::GetLightQueue(Vector3 position)
    {
        std::ranges::sort(
            lights,
            [&position](const Light* a, const Light* b)
            {
                if (a->lightType != b->lightType)
                {
                    if (a->lightType == LightType::Directional)
                        return true;
                    if (b->lightType == LightType::Directional)
                        return false;
                }

                const float aDistance = (a->GetGameObject()->GetPosition() - position).GetSqrMagnitude();
                const float bDistance = (b->GetGameObject()->GetPosition() - position).GetSqrMagnitude();
                return aDistance < bDistance;
            }
        );
        return lights;
    }

    LightType Light::GetLightType() const
    {
        return lightType;
    }
    Color Light::GetColor() const
    {
        return color;
    }
    float Light::GetIntensity() const
    {
        return intensity;
    }
    LightInfo Light::GetLightInfo(int order) const
    {
        const ObjectPtr<GameObject> gameObject = GetGameObject();
        const LightInfo lightInfo{
            Vector4(gameObject->GetPosition(), 1),
            Vector4(gameObject->GetFront().GetNormalized(), 1),
            color * intensity,
            gameObject->GetWorldToLocalMatrix(),
            Matrix4x4::Ortho(25, 25, -25, 25),
            order,
            lightType
        };

        return lightInfo;
    }
    ObjectPtr<Texture> Light::GetShadowMap() const
    {
        if (lightType == LightType::Point)
            return shadowMapCube.ToObjectPtr<Texture>();
        else
            return shadowMap.ToObjectPtr<Texture>();
    }

    void Light::SetType(LightType type)
    {
        this->lightType = type;
    }
    void Light::SetColor(Color color)
    {
        this->color = color;
    }
    void Light::SetIntensity(float intensity)
    {
        this->intensity = intensity;
    }

    void Light::UpdateShadowMap(const std::vector<Renderer*>& rendererQueue) const
    {
        const ObjectPtr<GameObject> gameObject = GetGameObject();
        LightInfo lightInfo = GetLightInfo(0);

        //渲染深度信息
        if (lightType == LightType::Point)
        {
            const Vector3 rotations[] = {
                {0, 90, 0}, {0, -90, 0},
                {-90, 0, 0}, {90, 0, 0},
                {0, 0, 0}, {0, 180, 0},
            };

            for (int index = 0; index < 6; index++)
            {
                shadowMapCube->SetRenderTarget(index);
                GL::Clear(true, true);

                lightInfo.worldToLightView = Matrix4x4::TRS(gameObject->GetPosition(), rotations[index], Vector3::one).GetInverse();
                lightInfo.viewToLightClip = Matrix4x4::Perspective(90, 1, 0.01f, 25);
                Graphics::SetLightInfo(lightInfo, nullptr);
                for (const ObjectPtr<Renderer>& renderer : rendererQueue)
                {
                    if (renderer->GetCastShadows() == false)
                        continue;

                    if (renderer->GetMaterialFallback()->UploadRP(PassType::ShadowCaster))
                        Graphics::DrawMesh(renderer->GetMesh());
                }
            }
        }
        else
        {
            shadowMap->SetRenderTarget();
            GL::Clear(true, true);

            Graphics::SetLightInfo(lightInfo, nullptr);
            for (const ObjectPtr<Renderer>& renderer : rendererQueue)
            {
                if (renderer->GetCastShadows() == false)
                    continue;

                if (renderer->GetMaterialFallback()->UploadRP(PassType::ShadowCaster))
                    Graphics::DrawMesh(renderer->GetMesh());
            }
        }

        Texture::SetRenderTargetDefault();
    }

    void Light::Transfer(Transferer& transferer)
    {
        Component::Transfer(transferer);

        transferer.TransferFieldOf<int>("lightType", lightType);
        transferer.TransferField("color", color);
        transferer.TransferField("intensity", intensity);
    }
    void Light::Awake()
    {
        Component::Awake();

        shadowMap = Texture2D::Create(2048, 2048, TextureFormat::R32G32B32A32_FLOAT);
        shadowMapCube = TextureCube::Create(512, 512, TextureFormat::R32G32B32A32_FLOAT);
    }
    void Light::Destroy()
    {
        DestroyImmediate(shadowMap);
        DestroyImmediate(shadowMapCube);

        Component::Destroy();
    }
    void Light::Enable()
    {
        Component::Enable();

        lights.emplace_back(this);
    }
    void Light::Disable()
    {
        erase(lights, this);

        Component::Disable();
    }
}
