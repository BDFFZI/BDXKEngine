#include "Camera.h"
#include <algorithm>
#include <cmath>
#include "BDXKEngine/Platform/IO/Resources.h"
#include "BDXKEngine/Function/Time.h"
#include "BDXKEngine/Function/Screen.h"
#include "BDXKEngine/Framework/Settings/GraphicsSettings.h"

namespace BDXKEngine
{
    float Camera::GetAspectRatio() const
    {
        const Vector2 viewSize = renderTarget.IsNull() ? Screen::GetSize() : renderTarget->GetSize();
        return viewSize.x / viewSize.y;
    }
    ObjectPtr<Texture2D> Camera::GetRenderTarget()
    {
        return renderTarget;
    }
    int Camera::GetDepth() const
    {
        return depth;
    }

    void Camera::SetRenderTarget(const ObjectPtr<Texture2D>& renderTarget)
    {
        this->renderTarget = renderTarget;
    }
    void Camera::SetDepth(int depth)
    {
        this->depth = depth;
    }
    void Camera::SetClearFlags(ClearFlags clearFlags)
    {
        this->clearFlags = clearFlags;
    }
    void Camera::SetBackground(Color color)
    {
        this->background = color;
    }
    void Camera::SetNearClipPlane(float distance)
    {
        switch (projection)
        {
        case Projection::Orthographic:
            nearClipPlane = distance;
            break;
        case Projection::Perspective: //不能为0,为0的话ClipPlaneParameter将出现除0而无法计算
            nearClipPlane = std::fmax(0.01f, distance);
            break;
        }
    }
    void Camera::SetFarClipPlane(float distance)
    {
        //必须和nearClipPlane有差异，不然后续参数计算会出现除0
        farClipPlane = std::fmax(nearClipPlane + 0.01f, distance);
    }

    void Camera::Render(const std::vector<ObjectPtr<Renderer>>& renderers)
    {
        //渲染排序 TODO
        //遮挡剔除 TODO

        //获取所有可渲染灯光,(优先级排序 TODO)
        std::vector<ObjectPtr<Light>>& lights = LightEditor::GetLights();
        const int lightsCount = static_cast<int>(lights.size());
        //解析灯光的Pass类型
        std::vector<PassType> lightPasses{static_cast<unsigned long long>(lightsCount)};
        if (lightsCount != 0)
        {
            //寻找最亮的方向光始或第一个光源，其始终使用ForwardBase
            int mainLight = 0;
            float intensity = 0;
            for (int i = 0; i < lightsCount; i++)
            {
                lightPasses[i] = PassType::ForwardAdd;
                const ObjectPtr<Light> light = lights[i];
                if (light->GetType() == LightType::Directional && light->GetIntensity() > intensity)
                {
                    mainLight = i;
                    intensity = light->GetIntensity();
                }
            }

            //将该灯光放在第一个渲染
            std::swap(lights[mainLight], lights[0]);
            lightPasses[0] = PassType::ForwardBase;
        }


        //开始渲染场景

        //渲染背景,在上传相机世界数据等之前使用，否则会覆盖数据
        switch (clearFlags)
        {
        case ClearFlags::Color:
            GL::Clear(true, true, background);
            break;
        case ClearFlags::Skybox:
            if (GraphicsSettings::skybox.IsNull() == false)
            {
                GL::Clear(true, true);
                Resources::GetSkyboxMaterial()->SetMatrix(0, transform->GetLocalToWorldMatrix());
                Resources::GetSkyboxMaterial()->SetVector(0, Vector4{transform->GetPosition(), 1});
                Resources::GetSkyboxMaterial()->SetPass(0);
                GL::SetTexture(6, GraphicsSettings::skybox.ToObjectPtr<Texture>());
                Graphics::DrawTexture(Resources::GetWhiteTexture(), {Vector2::zero, Screen::GetSize()});
            }
            else GL::Clear(true, true, background);
            break;
        case ClearFlags::Not:
            break;
        }

        //上传世界信息
        Graphics::UpdateWorldInfo(WorldInfo{
            {background * 0.5f},
            {{Time::GetRealtimeSinceStartup(), 0, 0}, 0}
        });

        //上传相机信息
        switch (projection)
        {
        case Projection::Orthographic:
            Graphics::UpdateCameraInfo(CameraInfo::Orthographic(
                transform->GetPosition(),
                transform->GetWorldToLocalMatrix(),
                GetAspectRatio(),
                nearClipPlane, farClipPlane,
                size));
            break;
        case Projection::Perspective:
            Graphics::UpdateCameraInfo(CameraInfo::Perspective(
                transform->GetPosition(),
                transform->GetWorldToLocalMatrix(),
                GetAspectRatio(),
                nearClipPlane, farClipPlane,
                fieldOfView));
            break;
        }

        //渲染物体，注意没灯光时不会触发渲染
        for (const ObjectPtr<Renderer>& renderer : renderers)
        {
            //上传物体信息至渲染管线
            const ObjectPtr<Transform> rendererTransform = renderer->GetGameObject()->GetTransform(); //获取物体矩阵
            Graphics::UpdateObjectInfo(ObjectInfo{rendererTransform->GetLocalToWorldMatrix()});

            //获取该物体的渲染管线资源
            const ObjectPtr<Mesh> mesh = renderer->GetMesh(); //获取网格
            const ObjectPtr<Material> material = renderer->GetMaterial(); //获取材质

            //每个灯光都需要单独渲染一遍
            for (int lightIndex = 0; lightIndex < lightsCount; lightIndex++)
            {
                //上传灯光和阴影信息至渲染管线
                Graphics::UpdateLightInfo(
                    LightEditor::GetLightInfo(lights[lightIndex], lightIndex),
                    LightEditor::GetShadowInfo(lights[lightIndex]),
                    LightEditor::GetShadowMap(lights[lightIndex])
                );

                //渲染符合该类型的Pass
                const PassType& lightPass = lightPasses[lightIndex]; //获取Pass类型，每一次光照都是一次Pass
                const int passCount = material->GetPassCount();
                for (int passIndex = 0; passIndex < passCount; passIndex++)
                {
                    if (material->GetPassType(passIndex) == lightPass)
                    {
                        material->SetPass(passIndex); //将该Pass信息上传渲染管线
                        Graphics::DrawMeshNow(mesh); //启动渲染管线进行渲染
                    }
                }
            }
        }

        //清理渲染管线
        GL::SetTexture(0, nullptr);
        GL::SetTexture(1, nullptr);
        GL::SetTexture(2, nullptr);
        GL::SetTexture(3, nullptr);
        //阴影贴图
        GL::SetTexture(4, nullptr);
        GL::SetTexture(5, nullptr);
        //天空盒
        GL::SetTexture(6, nullptr);
    }

    void Camera::Awake()
    {
        Component::Awake();

        transform = GetGameObject()->GetTransform();
    }
    
    void Camera::Transfer(Transferrer& transferrer)
    {
        Component::Transfer(transferrer);
        
        transferrer.TransferField("renderTarget",renderTarget);
        transferrer.TransferFieldOf<int>("clearFlags",clearFlags);
        transferrer.TransferFieldOf<int>("projection",projection);
        transferrer.TransferField("background",background);
        transferrer.TransferField("nearClipPlane",nearClipPlane);
        transferrer.TransferField("farClipPlane",farClipPlane);
        transferrer.TransferField("fieldOfView",fieldOfView);
        transferrer.TransferField("size",size);
        transferrer.TransferField("depth",depth);
    }
}
