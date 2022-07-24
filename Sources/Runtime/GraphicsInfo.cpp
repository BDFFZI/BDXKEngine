#include "GraphicsInfo.h"
#include "Debug.h"
using namespace BDXKEngine;

CameraInfo CameraInfo::Orthographic(Vector3 cameraPosition, Matrix4x4 worldToView, float aspectRatio, float nearClipPlane, float farClipPlane, float size)
{
	CameraInfo cameraInfo{};
	cameraInfo.worldToView = worldToView;
	cameraInfo.cameraPosition = { cameraPosition,1 };

	float unitClipPlaneHalfHeight = size;
	float ClipPlaneParameterA = 1 / (farClipPlane - nearClipPlane);
	float ClipPlaneParameterB = -ClipPlaneParameterA * nearClipPlane;
	cameraInfo.viewToClip = {
		1 / unitClipPlaneHalfHeight / aspectRatio,0,0,0,
		0,1 / unitClipPlaneHalfHeight,0,0,
		0,0,ClipPlaneParameterA,ClipPlaneParameterB,
		0,0,0,1
	};
	return cameraInfo;
}

CameraInfo CameraInfo::Perspective(Vector3 cameraPosition, Matrix4x4 worldToView, float aspectRatio, float nearClipPlane, float farClipPlane, float fieldOfView)
{
	CameraInfo cameraInfo{};
	cameraInfo.worldToView = worldToView;
	cameraInfo.cameraPosition = { cameraPosition,1 };

	float unitClipPlaneHalfHeight = std::tan(fieldOfView / 2 / 180 * M_PI);
	//裁剪面的作用是使当深度等于远截面时最终深度恰好为1，等于近截面时恰好为0
	//而最终深度计算结果=(az+b)/z
	//故我们的目标便是求该式中的a和b
	//列出二元一次方程组，利用代入消元法求解得出如下结论
	float ClipPlaneParameterB = farClipPlane * nearClipPlane / (nearClipPlane - farClipPlane);
	float ClipPlaneParameterA = -ClipPlaneParameterB / nearClipPlane;
	cameraInfo.viewToClip = {
		//控制视野范围并避免受窗口大小缩放影响
		1 / unitClipPlaneHalfHeight / aspectRatio ,0,0,0,
		0,1 / unitClipPlaneHalfHeight,0,0,
		0,0,ClipPlaneParameterA,ClipPlaneParameterB,
		//利用齐次坐标中的w分量实现近大远小公式 xy / z
		0,0,1,0
	};
	return cameraInfo;
}
