#include "Material.h"
#include <algorithm>

namespace BDXKEngine {
	Material::Material(std::vector<Shader*> shaders)
	{
		SetShaders(shaders);
		//创建图形资源
		parameters = {};
		GL::CreateConstantBuffer(&parameters, sizeof(Parameters), &parametersBuffer.p);
		GL::CreateSamplerState(&samplerState.p);
	}

	std::vector<Shader*> Material::GetShaders() {
		return shaders;
	}
	Material::RenderQueue Material::GetRenderQueue()
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
			throw std::exception("shader不允许为空");
		this->shaders = shader;
	}
	void Material::SetRenderQueue(RenderQueue renderQueue)
	{
		this->renderQueue = renderQueue;
	}
	void Material::SetTexture(int instanceID, Texture* texture)
	{
		textures[instanceID] = texture;
	}
	void Material::SetFloat(int instanceID, float value)
	{
		float* object = (float*)&parameters;
		*(object + instanceID) = value;
	}
	void Material::SetPass(int index)
	{
		//设置纹理
		for (auto index = textures.begin(); index != textures.end(); index++)
		{
			ID3D11ShaderResourceView* view = GetResourceView(index->second);
			GL::SetShaderResource(index->first, &view);
		}
		GL::SetSamplerState(&samplerState.p);
		//设置常量
		GL::UpdateBuffer(parametersBuffer.p, &parameters);
		GL::SetConstantBuffer(2, &parametersBuffer.p);
		//设置着色器
		ShaderEditor::SetPass(shaders[index]);
	}
}