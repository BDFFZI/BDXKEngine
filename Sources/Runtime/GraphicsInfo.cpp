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
	//�ü����������ʹ����ȵ���Զ����ʱ�������ǡ��Ϊ1�����ڽ�����ʱǡ��Ϊ0
	//��������ȼ�����=(az+b)/z
	//�����ǵ�Ŀ��������ʽ�е�a��b
	//�г���Ԫһ�η����飬���ô�����Ԫ�����ó����½���
	float ClipPlaneParameterB = farClipPlane * nearClipPlane / (nearClipPlane - farClipPlane);
	float ClipPlaneParameterA = -ClipPlaneParameterB / nearClipPlane;
	cameraInfo.viewToClip = {
		//������Ұ��Χ�������ܴ��ڴ�С����Ӱ��
		1 / unitClipPlaneHalfHeight / aspectRatio ,0,0,0,
		0,1 / unitClipPlaneHalfHeight,0,0,
		0,0,ClipPlaneParameterA,ClipPlaneParameterB,
		//������������е�w����ʵ�ֽ���ԶС��ʽ xy / z
		0,0,1,0
	};
	return cameraInfo;
}
