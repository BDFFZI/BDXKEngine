#pragma once
#include <map>
#include <vector>
#include "ObjectPtr.h"
#include "Shader.h"
#include "Texture.h"


namespace BDXKEngine {
	enum class RenderQueue {
		Background = 1000,
		Geometry = 2000,
		AlphaTest = 2450,
		Transparent = 3000,
		Overlay = 4000,
	};

	class Material :public Object, ShaderEditor
	{
	public:
		Material(std::vector<ObjectPtr<Shader>> shaders);

		std::vector<ObjectPtr<Shader>> GetShaders();
		RenderQueue GetRenderQueue();
		int GetPassCount();
		PassType GetPassType(int index);


		void SetShaders(std::vector<ObjectPtr<Shader>> shader);
		void SetRenderQueue(RenderQueue renderQueue);
		void SetTexture(int slotIndex, ObjectPtr<Texture> texture);
		void SetFloat(int slotIndex, float value);
		/// �õ�ǰ���ʵ������������Ⱦ����
		void SetPass(int index);
	private:
		struct Parameters {
			Vector4 parameter0;
			Vector4 parameter1;
			Vector4 parameter2;
			Vector4 parameter3;
		};

		std::vector<ObjectPtr<Shader>> shaders;
		std::vector<ObjectPtr<Texture>> textures;
		Parameters parameters = {};

		CComPtr<ID3D11SamplerState> samplerState;
		CComPtr<ID3D11Buffer> parametersBuffer;

		RenderQueue renderQueue = RenderQueue::Geometry;
	};
}
