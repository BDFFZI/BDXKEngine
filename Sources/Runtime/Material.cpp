#include "Material.h"
#include <algorithm>

namespace BDXKEngine {
	Material::Material(std::vector<Shader*> shaders)
	{
		SetShaders(shaders);
		//����ͼ����Դ
		parameters = {};
		GL::CreateConstantBuffer(&parameters, sizeof(Parameters), &parametersBuffer.p);
		GL::CreateSamplerState(&samplerState.p);
	}



	std::vector<Shader*> Material::GetShaders() {
		return shaders;
	}
	int Material::GetRenderQueue()
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

	void Material::SetShaders(std::vector<Shader*> shader)
	{
		if (shader.size() == 0)
			throw std::exception("shader������Ϊ��");
		this->shaders = shader;
	}
	void Material::SetTexture(int id, Texture* texture)
	{
		textures[id] = texture;
	}
	void Material::SetFloat(int id, float value)
	{
		float* target = (float*)&parameters;
		*(target + id) = value;
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
		GL::SetConstantBuffer(2, &parametersBuffer.p);
		//������ɫ��
		ShaderEditor::SetPass(shaders[index]);
	}
}