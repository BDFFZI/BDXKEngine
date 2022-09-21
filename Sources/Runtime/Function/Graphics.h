#pragma once
#include <functional>
#include "Resources.h"
#include "Platform/GL/GL.h"
#include "Platform/GL/GL2D.h"
#include "Platform/GL/Resources/Mesh.h"
#include "Platform/GL/Resources/Material.h"
#include "Platform/Window/Window.h"
#include "Platform/GL/Resources/Texture2D.h"
#include "Platform/GL/Resources/TextureCube.h"
#include "GraphicsInfo.h"

namespace BDXKEngine {
	class Graphics : protected GL2D
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

		static void DrawMeshNow(ObjectPtr<Mesh> mesh);
		static void DrawTexture(ObjectPtr<Texture2D> texture, Rect screenRect);
		static void Blit(ObjectPtr<Texture2D> source, ObjectPtr<Texture2D> dest, ObjectPtr<Material> material = Resources::GetBlitMaterial());
	protected:
		static void Initialize(Window* window);
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
	};
}


