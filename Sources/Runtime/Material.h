#pragma once
#include <map>
#include <vector>
#include "Shader.h"
#include "Texture.h"


namespace BDXKEngine {
	class Material :ShaderEditor, TextureEditor
	{
		struct Parameters {
			Vector4 parameter0;
			Vector4 parameter1;
			Vector4 parameter2;
			Vector4 parameter3;
		};
	public:
		Material(std::vector<Shader*> shaders);

		int FindPass(PassType passType);

		std::vector<Shader*> GetShaders();
		int GetPassCount();

		void SetShaders(std::vector<Shader*> shader);
		void SetTexture(int id, Texture* texture);
		void SetFloat(int id, float value);
		/// 用当前材质的所有物填充渲染管线
		void SetPass(int index);
	private:
		std::vector<Shader*> shaders;
		std::map<int, Texture*> textures;
		CComPtr<ID3D11SamplerState> samplerState;

		Parameters parameters;
		CComPtr<ID3D11Buffer> parametersBuffer;
	};
}
