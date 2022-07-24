#pragma once
#include "Color.h"
#include "Matrix4x4.h"
#include "Vector4.h"

namespace BDXKEngine {
	// LightInfo�д洢��ʱ���������Ϣ����ͬ�Ĺ��ռ���ʱ�Բ�����ʹ��������ͬ
	enum class LightType {
		Directional,
		Point,
		Spot,
		//Area
	};
	// ������Ⱦ�������羫�ȸ��ߵ����ع��գ����ʡ���ܵĶ������
	enum class RenderMode {
		Auto,
		Important,
		NotImportant,
	};

	struct WorldInfo {
		Color environment;
		Vector4 Time;
	};
	static_assert((sizeof(WorldInfo) % 16) == 0, "������������С������16�ֽڵı���");

	struct CameraInfo {
		static CameraInfo Orthographic(Vector3 cameraPosition, Matrix4x4 worldToView, float aspectRatio, float nearClipPlane, float farClipPlane, float size);
		static CameraInfo Perspective(Vector3 cameraPosition, Matrix4x4 worldToView, float aspectRatio, float nearClipPlane, float farClipPlane, float fieldOfView);

		Matrix4x4 worldToView = {};//TODO ��Ӧ��������Ӱ��
		Matrix4x4 viewToClip = {};
		Vector4 cameraPosition = {};
	};
	static_assert((sizeof(CameraInfo) % 16) == 0, "������������С������16�ֽڵı���");

	struct LightInfo {
		Vector4 lightPosition = {};
		Vector4 lightNormal = {};
		Color lightColor = {};
		Vector4 LightFactorFloat = {};
		LightType lightType = {};
		RenderMode lightRenderMode = {};
		int lightOrder = {};
		int space = {};
	};
	static_assert((sizeof(LightInfo) % 16) == 0, "������������С������16�ֽڵı���");

	struct ObjectInfo {
		Matrix4x4 objectToWorld;
	};
	static_assert((sizeof(ObjectInfo) % 16) == 0, "������������С������16�ֽڵı���");

	struct ShadowInfo {
		Matrix4x4 worldToLightView;
		Matrix4x4 viewToLightClip;
	};
	static_assert((sizeof(ShadowInfo) % 16) == 0, "������������С������16�ֽڵı���");
}
