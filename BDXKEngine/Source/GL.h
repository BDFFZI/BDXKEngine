#pragma once
#include <d3d11.h>
#include <dxgi1_6.h>
#include "Com.h"
#include "Assert.h"
#include "Color.h"

class GL
{
public:
	static void CreateDevice();
	static void CreateSwapChain(HWND hwnd);
	static void CreateRenderTexture();

	template<typename Vertex>
	static void CreateVertexBuffer(Vertex vertices[], int verticesSize, ID3D11Buffer** vertexBuffer)
	{
		//CD3D11_BUFFER_DESC vDesc = {};
		//vDesc.Usage = D3D11_USAGE_DYNAMIC;
		//vDesc.ByteWidth = verticesSize;
		//vDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		//vDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		CD3D11_BUFFER_DESC vDesc(verticesSize, D3D11_BIND_VERTEX_BUFFER);
		D3D11_SUBRESOURCE_DATA vData{ vertices,0,0 };

		Assert::IsSucceeded(
			device->CreateBuffer(&vDesc, &vData, vertexBuffer),
			L"创建顶点缓冲区失败"
		);

		//D3D11_MAPPED_SUBRESOURCE ms;
		//context->Map(*vertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
		//memcpy(ms.pData, vertices, verticesSize);                 // copy the data
		//context->Unmap(*vertexBuffer, NULL);
	}
	template<typename Index>
	static void CreateIndexBuffer(Index indexs[], int indexsSize, ID3D11Buffer** indexBuffer)
	{
		CD3D11_BUFFER_DESC iDesc(indexsSize, D3D11_BIND_INDEX_BUFFER);
		D3D11_SUBRESOURCE_DATA iData = { indexs ,0,0 };

		Assert::IsSucceeded(
			device->CreateBuffer(&iDesc, &iData, indexBuffer),
			L"创建索引缓冲区失败"
		);
	}

	static void CreateVertexShader(const char* path, D3D11_INPUT_ELEMENT_DESC vertexShaderInput[], int inputLayoutDescriptorCount, ID3D11VertexShader** vertexShader, ID3D11InputLayout** inputLayout);
	static void CreatePixelShader(const char* path, ID3D11PixelShader** pixelShader);

	static void Clear(Color color);
	static void Render(
		ID3D11Buffer* vertexBuffer, ID3D11InputLayout* inputLayout, int vertexSize,
		ID3D11Buffer* indexBuffer, DXGI_FORMAT indexFormat, int indexsCount,
		ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader,
		D3D11_PRIMITIVE_TOPOLOGY drawMode);

private:
	//struct RendererMatrix {
	//	DirectX::XMFLOAT4X4 localToWorld;
	//	DirectX::XMFLOAT4X4 worldToView;
	//	DirectX::XMFLOAT4X4 projection;
	//};
	//static_assert((sizeof(RendererMatrix) % 16) == 0, "Constant Buffer size must be 16-byte aligned");

	static CComPtr<ID3D11Device> device;
	static CComPtr<ID3D11DeviceContext> context;
	static CComPtr<IDXGISwapChain> swapChain;

	static CComPtr<ID3D11RenderTargetView> renderTargetView;
	static CComPtr<ID3D11DepthStencilView> depthStencilView;
};

