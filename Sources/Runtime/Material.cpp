#include "Material.h"
#include <algorithm>

namespace BDXKEngine {
	ObjectTransfer Material::Create(std::vector<ObjectPtr<Shader>> shaders)
	{
		ObjectPtr<Material> material = Instantiate<Material>(nullptr);
		material->SetShaders(shaders);
		material->textures.resize(4);
		material->parametersBuffer = Buffer::Create(BufferTarget::Constant, sizeof(Parameters));
		return material;
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
		this->shaders = shader;
	}
	void Material::SetRenderQueue(RenderQueue renderQueue)
	{
		this->renderQueue = renderQueue;
	}

	void Material::SetFloat(int slotIndex, float value)
	{
		if (slotIndex < 0 || slotIndex >= 8)
			throw std::exception("����������Χ");
		((float*)&parameters)[slotIndex] = value;
	}
	void Material::SetVector(int slotIndex, Vector4 value)
	{
		if (slotIndex < 0 || slotIndex >= 6)
			throw std::exception("����������Χ");
		((Vector4*)&parameters)[2 + slotIndex] = value;
	}
	void Material::SetMatrix(int slotIndex, Matrix4x4 value)
	{
		if (slotIndex < 0 || slotIndex >= 4)
			throw std::exception("����������Χ");
		((Matrix4x4*)&parameters)[2 + slotIndex] = value;
	}
	void Material::SetTexture(int slotIndex, ObjectPtr<Texture> texture)
	{
		if (slotIndex < 0 || slotIndex >= 4)
			throw std::exception("����������Χ");
		textures[slotIndex] = texture;
	}
	void Material::SetPass(int index)
	{
		//��������
		for (int index = 0; index < 4; index++)
			GL::SetTexture(index, textures[index]);

		//���ó���
		parametersBuffer->SetData(&parameters);
		GL::SetBuffer(0, parametersBuffer);

		//������ɫ��
		GL::SetShader(shaders[index]);
	}

	void Material::Import(ObjectTransfer& data)
	{
	}
	void Material::Export(ObjectTransfer& data)
	{
	}
}