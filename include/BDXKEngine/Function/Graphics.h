#pragma once
#include "BDXKEngine/Platform/IO/Resources.h"
#include "BDXKEngine/Platform/GL/GL2D.h"
#include "BDXKEngine/Platform/GL/Resources/Mesh.h"
#include "BDXKEngine/Platform/GL/Resources/Material.h"
#include "BDXKEngine/Platform/Window/Window.h"
#include "BDXKEngine/Platform/GL/Resources/Texture2D.h"
#include "BDXKEngine/Platform/GL/Resources/TextureCube.h"
#include "GraphicsInfo.h"

namespace BDXKEngine {
	class Graphics : protected GL2D
	{
	public:
		static void UpdateWorldInfo(WorldInfo worldInfo);
		static void UpdateCameraInfo(CameraInfo cameraInfo);
		static void UpdateObjectInfo(ObjectInfo objectInfo);
		static void UpdateLightInfo(LightInfo lightInfo, ShadowInfo shadowInfo, const ObjectPtr<Texture>& shadowMap);

		static void DrawMeshNow(const ObjectPtr<Mesh>& mesh);
		static void DrawTexture(ObjectPtr<Texture2D> texture, Rect screenRect);
		static void Blit(const ObjectPtr<Texture2D>& source, const ObjectPtr<Texture2D>& dest, const ObjectPtr<Material>& material = Resources::GetBlitMaterial());
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


