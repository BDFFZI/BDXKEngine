#pragma once
#include "BDXKEngine/Base/Data/Graphics/Color.h"
#include "BDXKEngine/Base/Data/Mathematics/Matrix/Matrix4x4.h"
#include "BDXKEngine/Base/Data/Mathematics/Vector/Vector4.h"

namespace BDXKEngine
{
    struct CameraInfo
    {
        static CameraInfo Orthographic(float aspectRatio, float nearClipPlane, float farClipPlane, float height,
                                       Matrix4x4 worldToView, Vector3 cameraPosition, Color environment, float time);
        static CameraInfo Perspective(float aspectRatio, float nearClipPlane, float farClipPlane, float fieldOfView,
                                      Matrix4x4 worldToView, Vector3 cameraPosition, Color environment, float time);

        Matrix4x4 worldToView = {}; //TODO 不应该受缩放影响
        Matrix4x4 viewToClip = {};
        Vector4 cameraPosition = {};
        Color environment = {};
        Vector4 Time = {};
    };

    static_assert((sizeof(CameraInfo) % 16) == 0, "常量缓冲区大小必须是16字节的倍数");

    // LightInfo中存储的时那类光照信息，不同的光照计算时对参数的使用有所不同
    enum class LightType
    {
        Directional,
        Point,
        Spot,
        //Area
    };

    struct LightInfo
    {
        Vector4 lightPosition = {};
        Vector4 lightDirection = {};
        Color lightColor = {};

        Matrix4x4 worldToLightView = {};
        Matrix4x4 viewToLightClip = {};

        int lightIndex = {};
        LightType lightType = {};
        Vector2 null;
    };

    static_assert((sizeof(LightInfo) % 16) == 0, "常量缓冲区大小必须是16字节的倍数");
}
