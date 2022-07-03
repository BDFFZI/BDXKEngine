#include "Material.h"
#include <algorithm>

// 用当前材质的所有物填充渲染管线

Material::Material(std::vector<Shader*> shaders)
{
	SetShaders(shaders);
	GL::CreateSamplerState(&samplerState.p);

	parameters = {};
	GL::CreateConstantBuffer(&parameters, sizeof(Parameters), &parametersBuffer.p);
}

int Material::FindPass(PassType passType)
{
	for (int i = 0; i < shaders.size(); i++)
	{
		if (shaders[i]->GetPassType() == passType)
			return i;
	}

	return -1;
}

std::vector<Shader*> Material::GetShaders() {
	return shaders;
}
int Material::GetPassCount()
{
	return (int)shaders.size();
}

void Material::SetShaders(std::vector<Shader*> shader)
{
	if (shader.size() == 0)
		throw std::exception("shader不允许为空");
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
	for (auto index = textures.begin(); index != textures.end(); index++)
	{
		ID3D11ShaderResourceView* view = GetResourceView(index->second);
		GL::SetShaderResource(index->first, &view);
	}
	GL::SetSamplerState(&samplerState.p);
	GL::UpdateBuffer(parametersBuffer.p, &parameters);
	GL::SetConstantBuffer(2, &parametersBuffer.p);
	ShaderEditor::SetPass(shaders[index]);
}
