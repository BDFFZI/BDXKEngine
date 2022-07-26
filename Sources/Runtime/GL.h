#pragma once
#include <d3d11_1.h>
#include <dxgi.h>
#include <cassert>
#include <atlbase.h>
#include "Color.h"
#include "Rect.h"
#include "ObjectPtr.h"

#include "Blend.h"
#include "Cull.h"
#include "ZTest.h"

namespace BDXKEngine {
	enum class GLRenderStage {
		Vertex,
		Pixed,
	};

	class Texture;
	class Texture2D;
	class Mesh;
	class GL
	{
	public:
		static void CreateVertexShader(const wchar_t* path, D3D11_INPUT_ELEMENT_DESC vertexShaderInput[], int inputLayoutDescriptorCount, ID3D11VertexShader** vertexShader, ID3D11InputLayout** inputLayout);
		static void CreatePixelShader(const wchar_t* path, ID3D11PixelShader** pixelShader);
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

		static void CreateSamplerState(ID3D11SamplerState** samplerState);

		static void UpdateBlend(Blend* blend);
		static void UpdateZTest(ZTest* zTest);
		template<typename BufferData>
		static void UpdateBuffer(ID3D11Buffer* buffer, BufferData* source)
		{
			context->UpdateSubresource(buffer, 0, nullptr, source, 0, 0);
		}

		/// 设置当前渲染管线中用于的着色器
		static void SetShader(ID3D11VertexShader* vertexShader, ID3D11InputLayout* vertexShaderInputLayout, ID3D11PixelShader* pixelShader);
		/// 设置当前渲染管线中的顶点索引数据
		static void SetMesh(ObjectPtr<Mesh> mesh);
		/// 设置当前渲染管线中的常量缓冲区，矩阵灯光数据之类的
		static void SetConstantBuffer(unsigned int startSlot, ID3D11Buffer** buffer);
		/// 设置当前渲染管线中的着色器资源,如纹理之类的
		static void SetTexture(unsigned int startSlot, ObjectPtr<Texture> texture);
		/// 设置着色器采样纹理时的方式
		static void SetSamplerState(ID3D11SamplerState** samplerState);
		/// 设置要渲染到的目标纹理，为空时设置回默认目标即屏幕,Present会导致解除绑定，所以每次都重新绑定
		static void SetRenderTarget(ObjectPtr<Texture2D> renderTargetTexture);
		/// 设置渲染时的颜色混合方式
		static void SetBlend(Blend* blend);
		/// 设置渲染时对深度模板缓冲区的影响方式
		static void SetZTest(ZTest* zTest);

		static void Clear(bool clearDepth, bool clearColor, Color backgroundColor = Color::clear, float depth = 1.0f);
		static void Render(int indexsCount);
	protected:
		static GL* Initialize(HWND window);

		static CComPtr<ID3D11Device> device;
		static CComPtr<ID3D11DeviceContext> context;
		static CComPtr<IDXGISwapChain1> swapChain;
		static D3D11_TEXTURE2D_DESC defaultRenderTargetDescription;
		static CComPtr<ID3D11RenderTargetView> defaultRenderTargetView;
		static CComPtr<ID3D11DepthStencilView> defaultDepthStencilView;
		//清除渲染和深度模板时用得上
		static CComPtr<ID3D11RenderTargetView> renderTargetView;
		static CComPtr<ID3D11DepthStencilView> depthStencilView;

		static void CreateDevice();
		static void CreateSwapChain(HWND hwnd);
		static void CompileShader(const wchar_t* path, const char* entrypoint, const char* object, ID3DBlob** blob);

		/// 若使用了GL2D功能，则可能出现因为渲染纹理被GL2D占用而导致无法重置纹理大小，所以要配合GL2D那边的资源释放
		static void ResizeDefaultRenderTarget(Rect rect = {});
		static CComPtr<ID3D11Texture2D> GetDefaultRenderTarget();

		/// 将默认渲染目标的画面呈现到屏幕上
		static void Present();
	};
}



