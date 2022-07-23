#include "Material.h"
#include <algorithm>

namespace BDXKEngine {
	Material::Material(std::vector<ObjectPtr<Shader>> shaders) :Object(L"New Material")
	{
		SetShaders(shaders);
		//����ͼ����Դ
		GL::CreateConstantBuffer(&parameters, sizeof(Parameters), &parametersBuffer.p);
		GL::CreateSamplerState(&samplerState.p);
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
			throw std::exception("shader������Ϊ��");
		this->shaders = shader;
	}
	void Material::SetRenderQueue(RenderQueue renderQueue)
	{
		this->renderQueue = renderQueue;
	}
	void Material::SetTexture(int slotIndex, ObjectPtr<Texture> texture)
	{
		textures[slotIndex] = texture;
	}
	void Material::SetFloat(int slotIndex, float value)
	{
		float* object = (float*)&parameters;
		*(object + slotIndex) = value;
	}
	void Material::SetPass(int index)
	{
		//��������
		for (auto index = textures.begin(); index != textures.end(); index++)
		{
			ID3D11ShaderResourceView* view = GetResourceView(index->second);
			GL::SetShaderResource(index->first, &view);
		}
		GL::SetSamplerState(&samplerState.p);
		//���ó���
		GL::UpdateBuffer(parametersBuffer.p, &parameters);
		GL::SetConstantBuffer(4, &parametersBuffer.p);
		//������ɫ��
		ShaderEditor::SetPass(shaders[index]);
	}
}