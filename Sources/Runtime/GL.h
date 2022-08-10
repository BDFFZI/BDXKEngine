#pragma once
#include <d3d11_1.h>
#include <dxgi.h>
#include <cassert>
#include <atlbase.h>
#include "Window.h"
#include "Color.h"
#include "Rect.h"
#include "ObjectPtr.h"

namespace BDXKEngine {
	class Texture;
	class Texture2D;
	class TextureCube;
	class Mesh;
	class Buffer;
	class Shader;
	class GL
	{
	public:
		static ObjectPtr<Texture2D> GetRenderTarget();

		/// 设置当前渲染管线中用于的着色器
		static void SetShader(ObjectPtr<Shader> shader);
		/// 设置当前渲染管线中的顶点索引数据
		static void SetMesh(ObjectPtr<Mesh> mesh);
		/// 设置当前渲染管线中的常量缓冲区，矩阵灯光数据之类的
		static void SetBuffer(unsigned int startSlot, ObjectPtr<Buffer> buffer);
		/// 设置当前渲染管线中的着色器资源,如纹理之类的
		static void SetTexture(unsigned int startSlot, ObjectPtr<Texture> texture);
		/// 设置要渲染到的目标纹理，为空时设置回默认目标即屏幕,Present会导致解除绑定，所以每次都重新绑定
		static void SetRenderTarget(ObjectPtr<Texture2D> renderTexture);
		static void SetRenderTarget(ObjectPtr<TextureCube> textureCube, int index);

		static void Clear(bool clearDepth, bool clearColor, Color backgroundColor = Color::clear, float depth = 1.0f);
		static void Render(int indexsCount);
	protected:
		static void Initialize(Window* window);

		static CComPtr<ID3D11Device> device;
		static CComPtr<ID3D11DeviceContext> context;
		static CComPtr<IDXGISwapChain1> swapChain;
		//清除渲染和深度模板时用得上
		static CComPtr<ID3D11RenderTargetView> renderTargetView;
		static CComPtr<ID3D11DepthStencilView> depthStencilView;
		static ObjectPtr<Texture2D> renderTexture;
		//默认资产
		static D3D11_TEXTURE2D_DESC defaultRenderTargetDescription;
		static CComPtr<ID3D11RenderTargetView> defaultRenderTargetView;
		static CComPtr<ID3D11DepthStencilView> defaultDepthStencilView;

		static void CreateDevice();
		static void CreateSwapChain(HWND hwnd);

		static CComPtr<ID3D11Texture2D> GetDefaultRenderTarget();

		/// 若使用了GL2D功能，则可能出现因为渲染纹理被GL2D占用而导致无法重置纹理大小，所以要配合GL2D那边的资源释放
		static void ResizeDefaultRenderTarget(Vector2 size);
		/// 将默认渲染目标的画面呈现到屏幕上
		static void Present();
	};
}



