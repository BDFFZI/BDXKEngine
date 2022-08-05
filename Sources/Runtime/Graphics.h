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
	class Graphics : protected GL2D
	{
	public:
		/// <summary>
		/// Ĭ�������HLSL����������˳��ʽ�������ݣ������������������������
		/// ��ᵼ����ɫ���õ��ľ���ᱻת�ã����Ǿͳ�������������
		/// </summary>
		/// <param name="matrix"></param>
		static void UpdateWorldInfo(WorldInfo worldInfo);
		static void UpdateCameraInfo(CameraInfo lightInfo);
		static void UpdateObjectInfo(ObjectInfo lightInfo);
		static void UpdateLightInfo(LightInfo lightInfo, ShadowInfo shadowInfo, ObjectPtr<Texture> shadowMap);

		static void DrawMeshNow(ObjectPtr<Mesh> mesh);
		static void DrawTexture(ObjectPtr<Texture2D> texture, Rect screenRect);
		static void Blit(ObjectPtr<Texture2D> source, ObjectPtr<Texture2D> dest, ObjectPtr<Material> material = Resources::GetBlitMaterial());
	protected:
		static Graphics* Initialize(Window* window);
	private:
		static ObjectPtr<Buffer> worldInfoBuffer;
		static ObjectPtr<Buffer> cameraInfoBuffer;
		static ObjectPtr<Buffer> lightInfoBuffer;
		static ObjectPtr<Buffer> objectInfoBuffer;
		static ObjectPtr<Buffer> shadowInfoBuffer;

		static Window* window;
		static ObjectPtr<Mesh> drawTextureMesh;
		static ObjectPtr<Texture> defaultTexture2D;
		static ObjectPtr<Texture> defaultTextureCube;

		static void OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter);
	};
}


