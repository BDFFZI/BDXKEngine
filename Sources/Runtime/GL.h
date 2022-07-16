#pragma once
#include <d3d11_1.h>
#include <dxgi.h>
#include <cassert>
#include <atlbase.h>
#include "Color.h"
#include "Rect.h"

#include "Blend.h"
#include "Cull.h"
#include "ZTest.h"

namespace BDXKEngine {
	enum class GLRenderStage {
		Vertex,
		Pixed,
	};

	class GL
	{
	public:
		static void CreateVertexShader(const wchar_t* path, D3D11_INPUT_ELEMENT_DESC vertexShaderInput[], int inputLayoutDescriptorCount, ID3D11VertexShader** vertexShader, ID3D11InputLayout** inputLayout);
		static void CreatePixelShader(const wchar_t* path, ID3D11PixelShader** pixelShader);
		
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
		static void CreateConstantBuffer(BufferData* source, int bufferSize, ID3D11Buffer** buffer)
		{
			CreateBuffer(source, bufferSize, D3D11_BIND_CONSTANT_BUFFER, buffer);
		}
		template<typename BufferData>
		static void CreateBuffer(BufferData* source, int bufferSize, D3D11_BIND_FLAG bindFlag, ID3D11Buffer** buffer)
		{
			CD3D11_BUFFER_DESC desc(bufferSize, bindFlag);
			D3D11_SUBRESOURCE_DATA data = { source ,0,0 };

			assert(SUCCEEDED(device->CreateBuffer(&desc, &data, buffer)));
		}

		static void CreateTexture2D(unsigned int width, unsigned int height, ID3D11Texture2D** texture2D, ID3D11ShaderResourceView** resourceView);
		static void CreateTexture2D(char* filePath)
		{
			//D3d11createtextu
		}
		static void CreateSamplerState(ID3D11SamplerState** samplerState);

		static void UpdateBlend(Blend* blend);
		static void UpdateZTest(ZTest* zTest);
		template<typename BufferData>
		static void UpdateBuffer(ID3D11Buffer* buffer, BufferData* source)
		{
			context->UpdateSubresource(buffer, 0, nullptr, source, 0, 0);
		}

		// 设置当前渲染管线中用于的着色器
		static void SetShader(ID3D11VertexShader* vertexShader, ID3D11InputLayout* vertexShaderInputLayout, ID3D11PixelShader* pixelShader);
		// 设置当前渲染管线中的顶点索引数据
		static void SetBuffer(ID3D11Buffer* vertexBuffer, int vertexSize, ID3D11Buffer* indexBuffer, DXGI_FORMAT indexFormat);
		// 设置当前渲染管线中的常量缓冲区，矩阵灯光数据之类的
		static void SetConstantBuffer(unsigned int startSlot, ID3D11Buffer** buffer);
		// 设置当前渲染管线中的着色器资源,如纹理之类的
		static void SetShaderResource(unsigned int startSlot, ID3D11ShaderResourceView** resource);
		// 设置着色器采样纹理时的方式
		static void SetSamplerState(ID3D11SamplerState** samplerState);
		// 设置要渲染到的目标纹理，为空时设置回默认目标，即屏幕
		static void SetRenderTexture(ID3D11Texture2D* renderTargetTexture);
		// 设置渲染时的颜色混合方式
		static void SetBlend(Blend* blend);
		// 设置渲染时对深度模板缓冲区的影响方式
		static void SetZTest(ZTest* zTest);



		/// TODO 有一个BUG，因为渲染纹理被GL2D占用着，故若没有和GL2D配合使用会导致无法重置纹理大小
		static void Viewport(Rect rect = {});
		static void Begin(D3D11_PRIMITIVE_TOPOLOGY drawMode);
		static void End();
		static void Clear(bool clearDepth, bool clearColor, Color backgroundColor = Color::clear, float depth = 1.0f);
		static void Render(int indexsCount);

		static CComPtr<ID3D11Texture2D> GetRenderTargetTexture();;
	protected:
		static GL* Initialize(HWND window);
	private:

		static CComPtr<ID3D11Device> device;
		static CComPtr<ID3D11DeviceContext> context;
		static CComPtr<IDXGISwapChain1> swapChain;
		//清除渲染和深度模板时用得上
		static CComPtr<ID3D11RenderTargetView> renderTargetView;
		static CComPtr<ID3D11DepthStencilView> depthStencilView;

		static void CreateDevice();
		static void CreateSwapChain(HWND hwnd);
		static void CompileShader(const wchar_t* path, const char* entrypoint, const char* target, ID3DBlob** blob);
	};
}



