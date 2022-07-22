#pragma once
#include <map>
#include <vector>
#include "ObjectPtr.h"
#include "Shader.h"
#include "Texture.h"


namespace BDXKEngine {
	class Material :public Object, ShaderEditor, TextureEditor
	{
	public:
		enum class RenderQueue {
			Background = 1000,
			Geometry = 2000,
			AlphaTest = 2450,
			Transparent = 3000,
			Overlay = 4000,
		};

		struct Parameters {
			Vector4 parameter0;
			Vector4 parameter1;
			Vector4 parameter2;
			Vector4 parameter3;
		};

		Material(std::vector<ObjectPtr<Shader>> shaders);


		std::vector<ObjectPtr<Shader>> GetShaders();
		RenderQueue GetRenderQueue();
		int GetPassCount();
		PassType GetPassType(int index);


		void SetShaders(std::vector<ObjectPtr<Shader>> shader);
		void SetRenderQueue(RenderQueue renderQueue);
		void SetTexture(int instanceID, ObjectPtr<Texture> texture);
		void SetFloat(int instanceID, float value);
		/// 用当前材质的所有物填充渲染管线
		void SetPass(int index);
	private:
		std::vector< ObjectPtr<Shader>> shaders;
		std::map<int, ObjectPtr<Texture>> textures;
		CComPtr<ID3D11SamplerState> samplerState;

		RenderQueue renderQueue;
		Parameters parameters;
		CComPtr<ID3D11Buffer> parametersBuffer;
	};
}
