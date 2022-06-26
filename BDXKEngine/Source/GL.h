#pragma once
#include <d3d11.h>
#include <dxgi1_6.h>
#include <cassert>
#include <atlbase.h>
#include "Color.h"

class GL
{
public:
	template<typename Vertex>
	static void CreateVertexBuffer(Vertex vertices[], int verticesSize, ID3D11Buffer** vertexBuffer)
	{
		CreateBuffer(vertices, verticesSize, D3D11_BIND_VERTEX_BUFFER, vertexBuffer);
	}
	template<typename Index>
	static void CreateIndexBuffer(Index indexs[], int indexsSize, ID3D11Buffer** indexBuffer)
	{
		CreateBuffer(indexs, indexsSize, D3D11_BIND_INDEX_BUFFER, indexBuffer);
	}
	template<typename BufferData>
	static void CreateBuffer(BufferData* source, int bufferSize, D3D11_BIND_FLAG bindFlag, ID3D11Buffer** buffer)
	{
		CD3D11_BUFFER_DESC desc(bufferSize, bindFlag);
		D3D11_SUBRESOURCE_DATA data = { source ,0,0 };

		assert(SUCCEEDED(device->CreateBuffer(&desc, &data, buffer)));
	}
	template<typename BufferData>
	static void UpdateBuffer(ID3D11Buffer* buffer, BufferData* source)
	{
		context->UpdateSubresource(buffer, 0, nullptr, source, 0, 0);
	}
	static void SetVertexConstantBuffer(ID3D11Buffer** buffer)
	{
		context->VSSetConstantBuffers(0, 1, buffer);
	}

	static void CreateVertexShader(const char* path, D3D11_INPUT_ELEMENT_DESC vertexShaderInput[], int inputLayoutDescriptorCount, ID3D11VertexShader** vertexShader, ID3D11InputLayout** inputLayout);
	static void CreatePixelShader(const char* path, ID3D11PixelShader** pixelShader);

	static void Clear(Color color, float depth, unsigned char stencil);
	static void Render(
		ID3D11Buffer* vertexBuffer, ID3D11InputLayout* inputLayout, int vertexSize,
		ID3D11Buffer* indexBuffer, DXGI_FORMAT indexFormat, int indexsCount,
		ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader,
		D3D11_PRIMITIVE_TOPOLOGY drawMode);

protected:
	static void Initialize(HWND window)
	{
		GL::CreateDevice();
		GL::CreateSwapChain(window);
		GL::CreateRenderTexture();
	}
private:
	static CComPtr<ID3D11Device> device;
	static CComPtr<ID3D11DeviceContext> context;
	static CComPtr<IDXGISwapChain> swapChain;

	static CComPtr<ID3D11RenderTargetView> renderTargetView;
	static CComPtr<ID3D11DepthStencilView> depthStencilView;

	static void CreateDevice();
	static void CreateSwapChain(HWND hwnd);
	static void CreateRenderTexture();
};

