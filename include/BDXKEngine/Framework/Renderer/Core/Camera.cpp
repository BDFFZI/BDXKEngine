#include "Camera.h"
#include <algorithm>
#include <cmath>
#include "RenderSettings.h"
#include "BDXKEngine/Function/Graphics/Graphics.h"
#include "BDXKEngine/Function/Time/Time.h"
#include "BDXKEngine/Function/Window/Screen.h"

namespace BDXKEngine
{
    std::vector<Camera*> Camera::cameras = {};

    const std::vector<Camera*>& Camera::GetCameraQueue()
    {
        std::ranges::sort(cameras, [](const Camera* a, const Camera* b)
        {
            return a->priority < b->priority;
        });
        return cameras;
    }
    CameraInfo Camera::GetCameraInfo() const
    {
        const ObjectPtr<GameObject> gameObject = GetGameObject();
        switch (projection)
        {
        case Projection::Orthographic:
            return CameraInfo::Orthographic(
                GetAspectRatio(), nearClipPlane, farClipPlane, size,
                gameObject->GetWorldToLocalMatrix(), gameObject->GetPosition(),
                background * 0.5f, Time::GetRealtimeSinceStartup()
            );
        case Projection::Perspective:
            return CameraInfo::Perspective(
                GetAspectRatio(), nearClipPlane, farClipPlane, fieldOfView,
                gameObject->GetWorldToLocalMatrix(), gameObject->GetPosition(),
                background * 0.5f, Time::GetRealtimeSinceStartup()
            );
        }

        throw std::exception("Projection枚举值越界");
    }
    ObjectPtr<Texture2D> Camera::GetRenderTarget()
    {
        return renderTarget;
    }
    ClearFlags Camera::GetClearFlags() const
    {
        return clearFlags;
    }
    Projection Camera::GetProjection() const
    {
        return projection;
    }
    const Color& Camera::GetBackground() const
    {
        return background;
    }
    int Camera::GetPriority() const
    {
        return priority;
    }
    float Camera::GetAspectRatio() const
    {
        const Vector2 viewSize = renderTarget.IsNull() ? Screen::GetSize() : renderTarget->GetSize();
        return viewSize.x / viewSize.y;
    }

    void Camera::SetRenderTarget(const ObjectPtr<Texture2D>& renderTarget)
    {
        this->renderTarget = renderTarget;
    }
    void Camera::SetProjection(Projection projection)
    {
        this->projection = projection;
    }
    void Camera::SetClearFlags(ClearFlags clearFlags)
    {
        this->clearFlags = clearFlags;
    }
    void Camera::SetBackground(Color color)
    {
        this->background = color;
    }
    void Camera::SetPriority(int depth)
    {
        this->priority = depth;
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

    void Camera::Render(const std::vector<Light*>& lightQueue, const std::vector<Renderer*>& rendererQueue) const
    {
        const ObjectPtr<GameObject> gameObject = GetGameObject();

        //渲染排序 TODO
        //遮挡剔除 TODO

        //渲染背景,在上传相机世界数据等之前使用，否则会覆盖数据
        switch (clearFlags)
        {
        case ClearFlags::Color:
            {
                GL::Clear(true, true, background);
                break;
            }
        case ClearFlags::Skybox:
            {
                GL::Clear(true, true);

                const auto& skyboxMaterial = RenderSettings::GetSingleton().GetSkyboxMaterial();
                skyboxMaterial->SetMatrix(0, gameObject->GetLocalToWorldMatrix());
                skyboxMaterial->SetVector(0, Vector4{gameObject->GetPosition(), 1});
                skyboxMaterial->SetPass(0);

                const Vector2 screenSize = Screen::GetSize();
                Graphics::SetCameraInfo(
                    CameraInfo::Orthographic(
                        screenSize.x / screenSize.y, 0, 1, screenSize.y,
                        Matrix4x4::identity, Vector3::zero, Color::black, 0
                    ), RenderSettings::GetSingleton().GetSkybox()
                );

                Graphics::DrawRect({Vector2::zero, screenSize});
                break;
            }
        case ClearFlags::Not:
            break;
        }

        //上传相机信息
        Graphics::SetCameraInfo(GetCameraInfo(), nullptr);

        //渲染物体，注意没灯光时不会触发渲染
        for (const Renderer* renderer : rendererQueue)
        {
            //获取该物体的渲染管线资源
            const ObjectPtr<Mesh> mesh = renderer->GetMesh(); //获取网格
            const ObjectPtr<Material> material = renderer->GetMaterial(); //获取材质

            //每个灯光都需要单独渲染一遍,每一次光照都是一次物体的一次Pass
            const int lightCount = static_cast<int>(lightQueue.size());
            for (int lightIndex = 0; lightIndex < lightCount; lightIndex++)
            {
                const Light* light = lightQueue[lightIndex];
                const PassType lightPass = lightIndex == 0 ? PassType::ForwardBase : PassType::ForwardAdd; //渲染符合该类型的Pass
                Graphics::SetLightInfo(light->GetLightInfo(lightIndex), light->GetShadowMap()); //上传灯光和阴影信息至渲染管线

                const int passCount = material->GetPassCount();
                for (int passIndex = 0; passIndex < passCount; passIndex++)
                {
                    if (material->GetPassType(passIndex) == lightPass)
                    {
                        material->SetPass(passIndex); //将该Pass信息上传渲染管线
                        Graphics::DrawMesh(mesh); //启动渲染管线进行渲染
                    }
                }
            }
        }

        Graphics::SetCameraInfoNull();
        Graphics::SetLightInfoNull();
        Material::SetPassNull();
    }

    void Camera::Transfer(Transferer& transferer)
    {
        Component::Transfer(transferer);

        TransferFieldInfo(renderTarget);
        TransferFieldInfoOf(clearFlags, int);
        TransferFieldInfoOf(projection, int);
        TransferFieldInfo(background);
        TransferFieldInfo(nearClipPlane);
        TransferFieldInfo(farClipPlane);
        TransferFieldInfo(fieldOfView);
        TransferFieldInfo(size);
        TransferFieldInfo(priority);
    }
    void Camera::OnEnable()
    {
        cameras.push_back(this);
    }
    void Camera::OnDisable()
    {
        std::erase(cameras, this);
    }
}
