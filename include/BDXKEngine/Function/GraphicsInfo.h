#pragma once
#include "BDXKEngine/Base/Color.h"
#include "BDXKEngine/Base/Matrix/Matrix4x4.h"
#include "BDXKEngine/Base/Vector/Vector4.h"

namespace BDXKEngine {
	// LightInfo中存储的时那类光照信息，不同的光照计算时对参数的使用有所不同
	enum class LightType {
		Directional,
		Point,
		Spot,
		//Area
	};
	// 控制渲染开销，如精度更高的像素光照，或节省性能的顶点光照
	enum class RenderMode {
		Auto,
		Important,
		NotImportant,
	};

	struct WorldInfo {
		Color environment;
		Vector4 Time;
	};
	static_assert((sizeof(WorldInfo) % 16) == 0, "常量缓冲区大小必须是16字节的倍数");

	struct CameraInfo {
		static CameraInfo Orthographic(Vector3 cameraPosition, Matrix4x4 worldToView, float aspectRatio, float nearClipPlane, float farClipPlane, float size);
		static CameraInfo Perspective(Vector3 cameraPosition, Matrix4x4 worldToView, float aspectRatio, float nearClipPlane, float farClipPlane, float fieldOfView);

		Matrix4x4 worldToView = {};//TODO 不应该受缩放影响
		Matrix4x4 viewToClip = {};
		Vector4 cameraPosition = {};
	};
	static_assert((sizeof(CameraInfo) % 16) == 0, "常量缓冲区大小必须是16字节的倍数");

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
	static_assert((sizeof(LightInfo) % 16) == 0, "常量缓冲区大小必须是16字节的倍数");

	struct ObjectInfo {
		Matrix4x4 objectToWorld;
	};
	static_assert((sizeof(ObjectInfo) % 16) == 0, "常量缓冲区大小必须是16字节的倍数");

	struct ShadowInfo {
		Matrix4x4 worldToLightView;
		Matrix4x4 viewToLightClip;
	};
	static_assert((sizeof(ShadowInfo) % 16) == 0, "常量缓冲区大小必须是16字节的倍数");
}
