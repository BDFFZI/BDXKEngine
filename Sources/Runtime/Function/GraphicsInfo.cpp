#include "GraphicsInfo.h"
#include "Base/Extension/Debug.h"
using namespace BDXKEngine;

CameraInfo CameraInfo::Orthographic(Vector3 cameraPosition, Matrix4x4 worldToView, float aspectRatio, float nearClipPlane, float farClipPlane, float height)
{
	CameraInfo cameraInfo{};
	cameraInfo.worldToView = worldToView;
	cameraInfo.cameraPosition = { cameraPosition,1 };
	cameraInfo.viewToClip = Matrix4x4::Ortho(height / 2 * aspectRatio, height / 2, nearClipPlane, farClipPlane);
	return cameraInfo;
}

CameraInfo CameraInfo::Perspective(Vector3 cameraPosition, Matrix4x4 worldToView, float aspectRatio, float nearClipPlane, float farClipPlane, float fieldOfView)
{
	CameraInfo cameraInfo{};
	cameraInfo.worldToView = worldToView;
	cameraInfo.cameraPosition = { cameraPosition,1 };
	cameraInfo.viewToClip = Matrix4x4::Perspective(fieldOfView, aspectRatio, nearClipPlane, farClipPlane);
	return cameraInfo;
}
