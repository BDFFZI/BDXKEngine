#include "Material.h"
#include <algorithm>

namespace BDXKEngine {
	ObjectPtr<Material> Material::Create(std::vector<ObjectPtr<Shader>> shaders)
	{
		return { Object::InstantiateNoAwake<Shader>(
			[=](Exporter& exporter) {
				exporter.TransferInt(static_cast<int>(RenderQueue::Geometry));

				exporter.TransferInt(shaders.size());
				for (ObjectPtr<Shader> shader : shaders)
					exporter.TransferObject(shader);

				ObjectPtr<Object> object = nullptr;
				exporter.TransferObject(object);
				exporter.TransferObject(object);
				exporter.TransferObject(object);
				exporter.TransferObject(object);

				Parameters parameters = {};
				exporter.TransferBytes(reinterpret_cast<char*>(&parameters), sizeof(Parameters));
			}
		) };
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
		(&texture0)[slotIndex] = texture;
	}
	void Material::SetPass(int index)
	{
		//设置纹理
		GL::SetTexture(0, texture0);
		GL::SetTexture(1, texture1);
		GL::SetTexture(2, texture2);
		GL::SetTexture(3, texture3);

		//设置常量
		parametersBuffer->SetData(reinterpret_cast<char*>(&parameters));
		GL::SetBuffer(0, parametersBuffer);

		//设置着色器
		GL::SetShader(shaders[index]);
	}

	void Material::Export(Exporter& exporter)
	{
		exporter.TransferInt(static_cast<int>(renderQueue));

		exporter.TransferInt(shaders.size());
		for (ObjectPtr<Shader>& shader : shaders)
			exporter.TransferObject(shader);

		exporter.TransferObject(texture0);
		exporter.TransferObject(texture1);
		exporter.TransferObject(texture2);
		exporter.TransferObject(texture3);
		exporter.TransferBytes(reinterpret_cast<char*>(&parameters), sizeof(Parameters));
	}
	void Material::Import(Importer& importer)
	{
		renderQueue = static_cast<RenderQueue>(importer.TransferInt());

		int shadersCount = importer.TransferInt();
		for (int i = 0; i < shadersCount; i++)
		{
			ObjectPtr<Shader> shader = nullptr;
			importer.TransferObject(shader);
			shaders.push_back(shader);
		}

		importer.TransferObject(texture0);
		importer.TransferObject(texture1);
		importer.TransferObject(texture2);
		importer.TransferObject(texture3);
		importer.TransferBytes(reinterpret_cast<char*>(&parameters), sizeof(Parameters));
	}
	void Material::Awake()
	{
		parametersBuffer = Buffer::Create(BufferTarget::Constant, sizeof(Parameters));
	}
}