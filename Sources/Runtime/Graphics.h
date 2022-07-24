#pragma once
#include <functional>
#include "GL.h"
#include "GL2D.h"
#include "Mesh.h"
#include "Material.h"
#include "Window.h"
#include "Texture2D.h"
#include "RenderEvent.h"
#include "GraphicsInfo.h"

namespace BDXKEngine {
	class Graphics :protected GL, protected GL2D, ShaderEditor
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
		static void UpdateShadowInfo(ShadowInfo shadowInfo,ObjectPtr<Texture2D> shadowMap);

		static void DrawMeshNow(ObjectPtr<Mesh> mesh);
		static void DrawTexture(Rect screenRect, ObjectPtr<Texture2D> texture);
		static void Blit(ObjectPtr<Texture2D> source, ObjectPtr<Texture2D> dest, ObjectPtr<Material> material = blitMaterial);
	protected:
		static Graphics* Initialize(Window* window, GL* gl, ObjectPtr<Material> blitMaterial, GL2D** gl2d);
	private:
		static WorldInfo worldInfo;
		static CameraInfo cameraInfo;
		static LightInfo lightInfo;
		static ObjectInfo objectInfo;
		static ShadowInfo shadowInfo;
		static ObjectPtr<Material> blitMaterial;

		static CComPtr<ID3D11Buffer> worldInfoBuffer;
		static CComPtr<ID3D11Buffer> cameraInfoBuffer;
		static CComPtr<ID3D11Buffer> lightInfoBuffer;
		static CComPtr<ID3D11Buffer> objectInfoBuffer;
		static CComPtr<ID3D11Buffer> shadowInfoBuffer;

		static void OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter);
	};
}


