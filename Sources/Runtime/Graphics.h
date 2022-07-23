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
	};
	static_assert((sizeof(WorldInfo) % 16) == 0, "������������С������16�ֽڵı���");

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
			//�ü����������ʹ����ȵ���Զ����ʱ�������ǡ��Ϊ1�����ڽ�����ʱǡ��Ϊ0
			//��������ȼ�����=(az+b)/z
			//�����ǵ�Ŀ��������ʽ�е�a��b
			//�г���Ԫһ�η����飬���ô�����Ԫ�����ó����½���
			float ClipPlaneParameterB = farClipPlane * nearClipPlane / (nearClipPlane - farClipPlane);
			float ClipPlaneParameterA = -ClipPlaneParameterB / nearClipPlane;
			cameraToView = {
				//������Ұ��Χ�������ܴ��ڴ�С����Ӱ��
				1 / unitClipPlaneHalfHeight / aspectRatio,0,0,0,
				0,1 / unitClipPlaneHalfHeight,0,0,
				0,0,ClipPlaneParameterA,ClipPlaneParameterB,
				//������������е�w����ʵ�ֽ���ԶС��ʽ xy / z
				0,0,1,0
			};
		}

		Matrix4x4 worldToCamera;
		Matrix4x4 cameraToView;
		Vector4 cameraPosition;
	};
	static_assert((sizeof(CameraInfo) % 16) == 0, "������������С������16�ֽڵı���");

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
	static_assert((sizeof(LightInfo) % 16) == 0, "������������С������16�ֽڵı���");

	struct ObjectInfo {
		Matrix4x4 localToWorld;
	};
	static_assert((sizeof(ObjectInfo) % 16) == 0, "������������С������16�ֽڵı���");

	class Graphics :protected GL, protected GL2D, MeshEditor, ShaderEditor, Texture2DEditor, RenderEventEditor
	{
	public:
		/// <summary>
		/// Ĭ�������HLSL����������˳��ʽ�������ݣ������������������������
		/// ��ᵼ����ɫ���õ��ľ���ᱻת�ã����Ǿͳ�������������
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


