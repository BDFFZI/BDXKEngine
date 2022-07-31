#pragma once
#include <map>
#include <functional>
#include "Window.h"
#include "Material.h"
#include "Texture2D.h"

#define GetResourcesPath(Type,Name) "../Resources/"#Type"/"#Name""
#define GetResourcesPathW(Type,Name) L"../Resources/"#Type"/"#Name""

namespace BDXKEngine {
	class Resources
	{
	public:
		static ObjectPtr<Material> GetShadowMapMaterial();
		static ObjectPtr<Material> GetSkyboxMaterial();
		static ObjectPtr<Material> GetBlitMaterial();
		static ObjectPtr<Texture2D> GetWhiteTexture();
		//template<typename TResult>
		//static ObjectPtr<TResult> Load(std::wstring path)
		//{
		//	if(
		//	return creator[typeid(TResult).hash_code()]
		//}
	protected:
		static ObjectPtr<Material> shadowMapMaterial;
		static ObjectPtr<Material> skyboxMaterial;
		static ObjectPtr<Material> blitMaterial;
		static ObjectPtr<Texture2D> whiteTexture;

		static Resources* Initialize(Window* window,GL* gl);
	private:
		static std::map<size_t, std::function<ObjectPtr<Object>(std::wstring path)>> creator;
	
		static void OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter);
	};
}

