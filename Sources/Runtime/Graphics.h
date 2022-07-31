#pragma once
#include <functional>
#include "Resources.h"
#include "GL.h"
#include "GL2D.h"
#include "Mesh.h"
#include "Material.h"
#include "Window.h"
#include "Texture2D.h"
#include "TextureCube.h"
#include "RenderEvent.h"
#include "GraphicsInfo.h"

namespace BDXKEngine {
	class Graphics : protected GL2D, ShaderEditor
	{
	public:
		/// <summary>
		/// 默认情况下HLSL以列向量的顺序方式接受数据，而矩阵传输是以行向量传入的
		/// 这会导致着色器得到的矩阵会被转置，于是就成了行向量矩阵
		/// </summary>
		/// <param name="matrix"></param>
		static void UpdateWorldInfo(WorldInfo worldInfo);
		static void UpdateCameraInfo(CameraInfo lightInfo);
		static void UpdateObjectInfo(ObjectInfo lightInfo);
		static void UpdateLightInfo(LightInfo lightInfo, ShadowInfo shadowInfo, ObjectPtr<Texture> shadowMap);
		static void UpdateShadowInfo(ShadowInfo shadowInfo);

		static void DrawMeshNow(ObjectPtr<Mesh> mesh);
		static void DrawTexture(ObjectPtr<Texture2D> texture, Rect screenRect);
		static void Blit(ObjectPtr<Texture2D> source, ObjectPtr<Texture2D> dest, ObjectPtr<Material> material = Resources::GetBlitMaterial());
	protected:
		static Graphics* Initialize(Window* window);
	private:
		static Window* window;
		static WorldInfo worldInfo;
		static CameraInfo cameraInfo;
		static LightInfo lightInfo;
		static ObjectInfo objectInfo;
		static ShadowInfo shadowInfo;
		static ObjectPtr<Mesh> drawTextureMesh;

		static CComPtr<ID3D11Buffer> worldInfoBuffer;
		static CComPtr<ID3D11Buffer> cameraInfoBuffer;
		static CComPtr<ID3D11Buffer> lightInfoBuffer;
		static CComPtr<ID3D11Buffer> objectInfoBuffer;
		static CComPtr<ID3D11Buffer> shadowInfoBuffer;

		static void OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter);
	};
}


