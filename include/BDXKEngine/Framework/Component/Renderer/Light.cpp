#include "Light.h"
#include "BDXKEngine/Base/Extension/Vector.h"
#include "BDXKEngine/Framework/GameObject.h"
#include "BDXKEngine/Framework/Component/Transform.h"
#include "BDXKEngine/Framework/Component/Renderer/RenderManager.h"

namespace BDXKEngine
{
    std::vector<ObjectPtr<Light>> Light::lights;

    LightType Light::GetType() const
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
    RenderMode Light::GetRenderMode() const
    {
        return renderMode;
    }

    void Light::SetLightType(LightType type)
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

    LightInfo Light::GetLightInfo(int order)
    {
        const ObjectPtr<Transform> lightTransform = GetGameObject()->GetTransform();
        const LightInfo lightInfo{
            Vector4(lightTransform->GetPosition(), 1),
            Vector4(lightTransform->GetFront().GetNormalized(), 1),
            color * intensity,
            {},
            lightType,
            renderMode,
            order
        };

        return lightInfo;
    }
    ShadowInfo Light::GetShadowInfo()
    {
        const ObjectPtr<Transform> transform = GetTransform();

        const ShadowInfo shadowInfo{
            transform->GetWorldToLocalMatrix(),
            CameraInfo::Orthographic(
                transform->GetPosition(),
                transform->GetWorldToLocalMatrix(),
                1, -25, 25, 25
            ).viewToClip
        };

        return shadowInfo;
    }
    ObjectPtr<Texture> Light::GetShadowMap()
    {
        if (lightType == LightType::Point)
            return shadowMapCube.ToObjectPtr<Texture>();
        else
            return shadowMap.ToObjectPtr<Texture>();
    }

    void Light::OnPreRenderFrame()
    {
        //获取投射阴影的物体
        const std::vector<ObjectPtr<Renderer>> renderers = RenderManager::GetRenderersQueue();
        //设置深度绘制着色器
        Resources::GetShadowMapMaterial()->SetPass(0);

        //渲染深度信息
        if (lightType == LightType::Point)
        {
            const ObjectPtr<Transform> transform = GetTransform();
            const Vector3 rotations[] = {
                {0, 90, 0}, {0, -90, 0},
                {-90, 0, 0}, {90, 0, 0},
                {0, 0, 0}, {0, 180, 0},
            };

            for (int index = 0; index < 6; index++)
            {
                Graphics::UpdateLightInfo(
                    GetLightInfo(),
                    ShadowInfo{
                        Matrix4x4::TRS(transform->GetPosition(), rotations[index], Vector3::one).GetInverse(),
                        Matrix4x4::Perspective(90, 1, 0.01f, 25)
                    },
                    nullptr
                );

                GL::SetRenderTarget(shadowMapCube, index);
                GL::Clear(true, true);

                for (const ObjectPtr<Renderer>& renderer : renderers)
                {
                    const ObjectPtr<Transform> rendererTransform = renderer->GetTransform();
                    Graphics::UpdateObjectInfo({rendererTransform->GetLocalToWorldMatrix()});
                    Graphics::DrawMeshNow(renderer->GetMesh());
                }
            }
        }
        else
        {
            //设置灯光阴影信息
            Graphics::UpdateLightInfo(
                GetLightInfo(),
                GetShadowInfo(),
                nullptr
            );

            //设置阴影贴图
            GL::SetRenderTarget(shadowMap);
            GL::Clear(true, true);

            for (const ObjectPtr<Renderer>& renderer : renderers)
            {
                const ObjectPtr<Transform> rendererTransform = renderer->GetTransform();
                Graphics::UpdateObjectInfo({rendererTransform->GetLocalToWorldMatrix()});
                Graphics::DrawMeshNow(renderer->GetMesh());
            }
        }

        GL::SetRenderTarget(nullptr);
    }

    void Light::Enable()
    {
        lights.emplace_back(this);

        Component::Enable();
    }
    void Light::Disable()
    {
        Vector::Remove(lights, {this});

        Component::Disable();
    }

    void Light::Transfer(Transferrer& transferrer)
    {
        Component::Transfer(transferrer);

        transferrer.TransferBytes("lightType", &lightType, sizeof(LightType));
        transferrer.TransferBytes("renderMode", &renderMode, sizeof(RenderMode));
        transferrer.TransferColor("color", color);
        transferrer.TransferFloat("intensity", intensity);
    }
    void Light::Awake()
    {
        shadowMap = Texture2D::Create(1024, 1024);
        shadowMapCube = TextureCube::Create(512, 512);

        Component::Awake();
    }
    void Light::Destroy()
    {
        Object::Destroy(shadowMap.ToObjectBase());
        Object::Destroy(shadowMapCube.ToObjectBase());

        Component::Destroy();
    }

    LightInfo LightEditor::GetLightInfo(const ObjectPtr<Light>& light, int order)
    {
        return light->GetLightInfo(order);
    }
    ShadowInfo LightEditor::GetShadowInfo(const ObjectPtr<Light>& light)
    {
        return light->GetShadowInfo();
    }
    ObjectPtr<Texture> LightEditor::GetShadowMap(const ObjectPtr<Light>& light)
    {
        return light->GetShadowMap();
    }
    std::vector<ObjectPtr<Light>>& LightEditor::GetLights()
    {
        return Light::lights;
    }
}
