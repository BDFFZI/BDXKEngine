#pragma once
#include <map>
#include <vector>
#include "ObjectPtr.h"
#include "Shader.h"
#include "Buffer.h"
#include "Texture.h"


namespace BDXKEngine {
	enum class RenderQueue {
		Background = 1000,
		Geometry = 2000,
		AlphaTest = 2450,
		Transparent = 3000,
		Overlay = 4000,
	};

	class Material :public Object
	{
	public:
		Material(std::vector<ObjectPtr<Shader>> shaders);

		std::vector<ObjectPtr<Shader>> GetShaders();
		RenderQueue GetRenderQueue();
		int GetPassCount();
		PassType GetPassType(int index);


		void SetShaders(std::vector<ObjectPtr<Shader>> shader);
		void SetRenderQueue(RenderQueue renderQueue);
		void SetFloat(int slotIndex, float value);
		void SetVector(int slotIndex, Vector4 value);
		void SetMatrix(int slotIndex, Matrix4x4 value);
		void SetTexture(int slotIndex, ObjectPtr<Texture> texture);
		/// 用当前材质的所有物填充渲染管线
		void SetPass(int index);
	private:
		struct Parameters {
			Vector4 float0_3;
			Vector4 float4_7;
			Vector4 vector0;
			Vector4 vector1;
			Vector4 vector2;
			Vector4 vector3;
			Vector4 vector4;
			Vector4 vector5;
			Matrix4x4 matrix0;
			Matrix4x4 matrix1;
			Matrix4x4 matrix2;
			Matrix4x4 matrix3;
		};

		RenderQueue renderQueue = RenderQueue::Geometry;

		std::vector<ObjectPtr<Shader>> shaders;
		std::vector<ObjectPtr<Texture>> textures;

		Parameters parameters;
		ObjectPtr<Buffer> parametersBuffer;
	};
}
