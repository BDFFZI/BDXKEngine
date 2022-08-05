#include "Material.h"
#include <algorithm>

namespace BDXKEngine {
	Material::Material(std::vector<ObjectPtr<Shader>> shaders) :Object(L"New Material")
	{
		SetShaders(shaders);
		textures.resize(4);
		parametersBuffer = new Buffer(BufferTarget::Constant, sizeof(Parameters));
	}

	std::vector<ObjectPtr<Shader>> Material::GetShaders() {
		return shaders;
	}
	RenderQueue Material::GetRenderQueue()
	{
		return renderQueue;
	}
	int Material::GetPassCount()
	{
		return (int)shaders.size();
	}
	PassType Material::GetPassType(int index)
	{
		return shaders[index]->GetPassType();
	}

	void Material::SetShaders(std::vector<ObjectPtr<Shader>> shader)
	{
		if (shader.size() == 0)
			throw std::exception("shader不允许为空");
		this->shaders = shader;
	}
	void Material::SetRenderQueue(RenderQueue renderQueue)
	{
		this->renderQueue = renderQueue;
	}

	void Material::SetFloat(int slotIndex, float value)
	{
		if (slotIndex < 0 || slotIndex >= 8)
			throw std::exception("超出容量范围");
		((float*)&parameters)[slotIndex] = value;
	}
	void Material::SetVector(int slotIndex, Vector4 value)
	{
		if (slotIndex < 0 || slotIndex >= 6)
			throw std::exception("超出容量范围");
		((Vector4*)&parameters)[2 + slotIndex] = value;
	}
	void Material::SetMatrix(int slotIndex, Matrix4x4 value)
	{
		if (slotIndex < 0 || slotIndex >= 4)
			throw std::exception("超出容量范围");
		((Matrix4x4*)&parameters)[2 + slotIndex] = value;
	}
	void Material::SetTexture(int slotIndex, ObjectPtr<Texture> texture)
	{
		if (slotIndex < 0 || slotIndex >= 4)
			throw std::exception("超出容量范围");
		textures[slotIndex] = texture;
	}
	void Material::SetPass(int index)
	{
		//设置纹理
		for (int index = 0; index < 4; index++)
			GL::SetTexture(index, textures[index]);

		//设置常量
		parametersBuffer->SetData(&parameters);
		GL::SetBuffer(0, parametersBuffer);

		//设置着色器
		GL::SetShader(shaders[index]);
	}
}