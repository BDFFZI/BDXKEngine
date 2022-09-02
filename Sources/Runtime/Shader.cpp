#include "Shader.h"
#include <d3dcompiler.h>

namespace BDXKEngine {
	void CompileShader(const wchar_t* path, const char* entrypoint, const char* object, ID3DBlob** blob) {
		CComPtr<ID3DBlob> compileError;
		HRESULT result = D3DCompileFromFile(path,
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entrypoint,
			object,
			D3DCOMPILE_DEBUG,
			0,
			blob,
			&compileError.p
		);

		if (FAILED(result))
		{
			char* errorInfo = (char*)compileError->GetBufferPointer();
			throw std::exception("编译着色器失败");
		}
	}

	ObjectPtr<Shader> Shader::Create(std::wstring vertexShaderhlsl, std::wstring pixelShaderhlsl, PassType passType)
	{
		return { Object::InstantiateNoAwake<Shader>(
			[=](Exporter& exporter) {
				exporter.TransferString(vertexShaderhlsl);
				exporter.TransferString(pixelShaderhlsl);
				exporter.TransferInt(static_cast<int>(passType));
				exporter.TransferBytes(reinterpret_cast<char*>(&Blend::Opaque), sizeof(Blend));
				exporter.TransferBytes(reinterpret_cast<char*>(&ZTest::Default), sizeof(ZTest));
			}
		) };
	}

	PassType Shader::GetPassType()
	{
		return passType;
	}
	Blend Shader::GetBlend()
	{
		return blend;
	}
	ZTest Shader::GetZTest()
	{
		return zTest;
	}

	void Shader::SetPassType(PassType passType)
	{
		this->passType = passType;
	}
	void Shader::SetBlend(Blend blend)
	{
		blendState = nullptr;
		this->blend = blend;
		D3D11_RENDER_TARGET_BLEND_DESC renderTargetBlendDescription{};
		renderTargetBlendDescription.BlendEnable = blend.state;
		renderTargetBlendDescription.SrcBlend = (D3D11_BLEND)blend.sourceFactor;
		renderTargetBlendDescription.DestBlend = (D3D11_BLEND)blend.destinationFactor;
		renderTargetBlendDescription.BlendOp = (D3D11_BLEND_OP)blend.operation;
		renderTargetBlendDescription.SrcBlendAlpha = D3D11_BLEND_ONE;
		renderTargetBlendDescription.DestBlendAlpha = D3D11_BLEND_ONE;
		renderTargetBlendDescription.BlendOpAlpha = D3D11_BLEND_OP_MAX;
		renderTargetBlendDescription.RenderTargetWriteMask = 15;
		D3D11_BLEND_DESC blendDescription{};
		blendDescription.RenderTarget[0] = renderTargetBlendDescription;
		HRESULT result = device->CreateBlendState(&blendDescription, &blendState.p);
		assert(SUCCEEDED(result));
	}
	void Shader::SetZTest(ZTest zTest)
	{
		depthStencilState = nullptr;
		this->zTest = zTest;
		D3D11_DEPTH_STENCIL_DESC description{};
		description.DepthEnable = true;
		description.DepthWriteMask = zTest.write ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
		description.DepthFunc = (D3D11_COMPARISON_FUNC)zTest.operation;
		HRESULT result = device->CreateDepthStencilState(&description, &depthStencilState.p);
		assert(SUCCEEDED(result));
	}

	void Shader::Import(Importer& importer)
	{
		vertexShaderhlsl = importer.TransferString();
		pixelShaderhlsl = importer.TransferString();
		passType = static_cast<PassType>(importer.TransferInt());
		importer.TransferBytes(reinterpret_cast<char*>(&blend), sizeof(Blend));
		importer.TransferBytes(reinterpret_cast<char*>(&zTest), sizeof(ZTest));
	}
	void Shader::Export(Exporter& exporter)
	{
		exporter.TransferString(vertexShaderhlsl);
		exporter.TransferString(pixelShaderhlsl);
		exporter.TransferInt(static_cast<int>(passType));
		exporter.TransferBytes(reinterpret_cast<char*>(&blend), sizeof(Blend));
		exporter.TransferBytes(reinterpret_cast<char*>(&zTest), sizeof(ZTest));
	}
	void Shader::Awake()
	{
		HRESULT result;
		//编译顶点着色器
		CComPtr<ID3DBlob> vertexBlob;
		CompileShader(vertexShaderhlsl.c_str(), "main", "vs_5_0", &vertexBlob.p);
		//创建顶点着色器
		result = device->CreateVertexShader(
			vertexBlob->GetBufferPointer(),
			vertexBlob->GetBufferSize(),
			nullptr,
			&vertexShader.p
		);
		assert(SUCCEEDED(result));

		//编译像素着色器
		CComPtr<ID3DBlob> pixelBlob;
		CompileShader(pixelShaderhlsl.c_str(), "main", "ps_5_0", &pixelBlob.p);
		//创建像素着色器
		result = device->CreatePixelShader(
			pixelBlob->GetBufferPointer(),
			pixelBlob->GetBufferSize(),
			nullptr,
			&pixelShader.p
		);
		assert(SUCCEEDED(result));

		//创建语义信息
		result = device->CreateInputLayout(
			VertexDescription,
			ARRAYSIZE(VertexDescription),
			vertexBlob->GetBufferPointer(),
			vertexBlob->GetBufferSize(),
			&inputLayout.p
		);
		assert(SUCCEEDED(result));

		SetPassType(passType);
		//初始化管线选项
		SetBlend(blend);
		SetZTest(zTest);
	}
}