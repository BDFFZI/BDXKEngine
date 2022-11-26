#include "GraphicsInfo.h"
using namespace BDXKEngine;

CameraInfo CameraInfo::Orthographic(float aspectRatio, float nearClipPlane, float farClipPlane, float height,
                                    Matrix4x4 worldToView, Vector3 cameraPosition, Color environment, float time)
{
    CameraInfo cameraInfo{};
    cameraInfo.viewToClip = Matrix4x4::Ortho(height / 2 * aspectRatio, height / 2, nearClipPlane, farClipPlane);
    cameraInfo.worldToView = worldToView;
    cameraInfo.cameraPosition = {cameraPosition, 1};
    cameraInfo.environment = environment;
    cameraInfo.Time = {time, 0, 0, 0};
    return cameraInfo;
}

CameraInfo CameraInfo::Perspective(float aspectRatio, float nearClipPlane, float farClipPlane, float fieldOfView,
                                   Matrix4x4 worldToView, Vector3 cameraPosition, Color environment, float time)
{
    CameraInfo cameraInfo{};
    cameraInfo.viewToClip = Matrix4x4::Perspective(fieldOfView, aspectRatio, nearClipPlane, farClipPlane);
    cameraInfo.worldToView = worldToView;
    cameraInfo.cameraPosition = {cameraPosition, 1};
    cameraInfo.environment = environment;
    cameraInfo.Time = {time, 0, 0, 0};
    return cameraInfo;
}
