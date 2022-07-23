#pragma once
#include <functional>
#include "GL.h"
#include "GL2D.h"
#include "Mesh.h"
#include "Material.h"
#include "Window.h"
#include "Texture2D.h"
#include "RenderEvent.h"

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
	};
	static_assert((sizeof(WorldInfo) % 16) == 0, "常量缓冲区大小必须是16字节的倍数");

	struct CameraInfo {
		CameraInfo()
		{
			worldToCamera = {};
			cameraToView = {};
			cameraPosition = {};
		}

		CameraInfo(Matrix4x4 worldToCamera, float fieldOfView, float aspectRatio, float nearClipPlane, float farClipPlane)
		{
			this->worldToCamera = worldToCamera;
			this->cameraPosition = { -Vector3{worldToCamera.m03,worldToCamera.m13,worldToCamera.m23},1 };

			float unitClipPlaneHalfHeight = std::tan(fieldOfView / 2 / 180 * M_PI);
			//裁剪面的作用是使当深度等于远界面时最终深度恰好为1，等于近截面时恰好为0
			//而最终深度计算结果=(az+b)/z
			//故我们的目标便是求该式中的a和b
			//列出二元一次方程组，利用代入消元法求解得出如下结论
			float ClipPlaneParameterB = farClipPlane * nearClipPlane / (nearClipPlane - farClipPlane);
			float ClipPlaneParameterA = -ClipPlaneParameterB / nearClipPlane;
			cameraToView = {
				//控制视野范围并避免受窗口大小缩放影响
				1 / unitClipPlaneHalfHeight / aspectRatio,0,0,0,
				0,1 / unitClipPlaneHalfHeight,0,0,
				0,0,ClipPlaneParameterA,ClipPlaneParameterB,
				//利用齐次坐标中的w分量实现近大远小公式 xy / z
				0,0,1,0
			};
		}

		Matrix4x4 worldToCamera;
		Matrix4x4 cameraToView;
		Vector4 cameraPosition;
	};
	static_assert((sizeof(CameraInfo) % 16) == 0, "常量缓冲区大小必须是16字节的倍数");

	struct LightInfo {
		Vector4 lightPosition;
		Vector4 lightNormal;
		Color lightColor;
		Vector4 LightFactorFloat;
		LightType lightType;
		RenderMode lightRenderMode;
		int lightOrder;
		int space;
	};
	static_assert((sizeof(LightInfo) % 16) == 0, "常量缓冲区大小必须是16字节的倍数");

	struct ObjectInfo {
		Matrix4x4 localToWorld;
	};
	static_assert((sizeof(ObjectInfo) % 16) == 0, "常量缓冲区大小必须是16字节的倍数");

	class Graphics :protected GL, protected GL2D, MeshEditor, ShaderEditor, Texture2DEditor, RenderEventEditor
	{
	public:
		/// <summary>
		/// 默认情况下HLSL以列向量的顺序方式接受数据，而矩阵传输是以行向量传入的
		/// 这会导致着色器得到的矩阵会被转置，于是就成了行向量矩阵
		/// </summary>
		/// <param name="matrix"></param>
		static void UpdateWorldInfo(WorldInfo worldInfo);
		static void UpdateCameraInfo(CameraInfo lightInfo);
		static void UpdateLightInfo(LightInfo lightInfo);
		static void UpdateObjectInfo(ObjectInfo lightInfo);

		static void DrawMeshNow(ObjectPtr<Mesh> mesh);
		static void SetRenderTarget(ObjectPtr<Texture2D> texture);
	protected:
		static Graphics* Initialize(Window* window, GL** gl, GL2D** gl2d);
	private:
		static WorldInfo worldInfo;
		static CameraInfo cameraInfo;
		static LightInfo lightInfo;
		static ObjectInfo objectInfo;

		static CComPtr<ID3D11Buffer> worldInfoBuffer;
		static CComPtr<ID3D11Buffer> cameraInfoBuffer;
		static CComPtr<ID3D11Buffer> lightInfoBuffer;
		static CComPtr<ID3D11Buffer> objectInfoBuffer;

		static void OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter);
		static CComPtr<IDXGISurface> GetIDXGISurface();
	};
}


