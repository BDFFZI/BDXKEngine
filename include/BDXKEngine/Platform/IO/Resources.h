#pragma once
#include <map>
#include "BDXKEngine/Platform/Window/Window.h"
#include "BDXKEngine/Platform/GL/Resources/Material.h"
#include "BDXKEngine/Platform/GL/Resources/Texture2D.h"
#include "BDXKEngine/Platform/GL/Resources/Mesh.h"

#define GetResourcesPath(Type,Name) "../Resources/"#Type"/"#Name""
#define GetResourcesPathW(Type,Name) "../Resources/"#Type"/"#Name""

namespace BDXKEngine {
	class Resources
	{
	public:
		static ObjectPtr<Material> GetShadowMapMaterial();
		static ObjectPtr<Material> GetSkyboxMaterial();
		static ObjectPtr<Material> GetBlitMaterial();
		static ObjectPtr<Texture2D> GetWhiteTexture();
		static ObjectPtr<Mesh> GetCubeMesh();
		static ObjectPtr<Mesh> GetSphereMesh();
		static ObjectPtr<Mesh> GetBlenderMesh();
		//template<typename TResult>
		//static ObjectPtr<TResult> Load(std::string path)
		//{
		//	if(
		//	return creator[typeid(TResult).hash_code()]
		//}
	protected:
		static ObjectPtr<Material> shadowMapMaterial;
		static ObjectPtr<Material> skyboxMaterial;
		static ObjectPtr<Material> blitMaterial;
		static ObjectPtr<Texture2D> whiteTexture;
		static ObjectPtr<Mesh> cubeMesh;
		static ObjectPtr<Mesh> sphereMesh;
		static ObjectPtr<Mesh> blenderMesh;

		static void Initialize(Window* window);
	private:
		static std::map<size_t, std::function<ObjectPtr<BDXKObject>(std::string path)>> creator;
	};
}

