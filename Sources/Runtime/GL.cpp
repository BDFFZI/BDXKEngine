#include "GL.h"
#include <d3dcompiler.h>
#include <exception>
#include "Texture2D.h"
#include "TextureCube.h"
#include "Mesh.h"

namespace BDXKEngine {

	void GL::CreateVertexShader(const wchar_t* path, D3D11_INPUT_ELEMENT_DESC inputLayoutDescriptor[], int inputLayoutDescriptorCount,
		ID3D11VertexShader** vertexShader, ID3D11InputLayout** inputLayout)
	{
		HRESULT result;

		CComPtr<ID3DBlob> blob;
		CompileShader(path, "main", "vs_5_0", &blob.p);
		//����������ɫ��
		result = device->CreateVertexShader(
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			nullptr,
			vertexShader
		);
		assert(SUCCEEDED(result));

		//����������Ϣ
		result = device->CreateInputLayout(
			inputLayoutDescriptor,
			inputLayoutDescriptorCount,
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			inputLayout
		);
		assert(SUCCEEDED(result));
	}
	void GL::CreatePixelShader(const wchar_t* path, ID3D11PixelShader** pixelShader)
	{
		CComPtr<ID3DBlob> blob;
		CompileShader(path, "main", "ps_5_0", &blob.p);

		//����������ɫ��
		assert(SUCCEEDED(device->CreatePixelShader(
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			nullptr,
			pixelShader
		)));
	}
	void GL::CreateSamplerState(ID3D11SamplerState** samplerState)
	{
		D3D11_SAMPLER_DESC samplerDescription{};
		samplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		device->CreateSamplerState(&samplerDescription, samplerState);
	}

	ObjectPtr<Texture2D> GL::GetRenderTarget()
	{
		return renderTexture;
	}

	void GL::UpdateBlend(Blend* blend)
	{
		D3D11_RENDER_TARGET_BLEND_DESC renderTargetBlendDescription{};
		renderTargetBlendDescription.BlendEnable = blend->state;
		renderTargetBlendDescription.SrcBlend = (D3D11_BLEND)blend->sourceFactor;
		renderTargetBlendDescription.DestBlend = (D3D11_BLEND)blend->destinationFactor;
		renderTargetBlendDescription.BlendOp = (D3D11_BLEND_OP)blend->operation;
		renderTargetBlendDescription.SrcBlendAlpha = D3D11_BLEND_ONE;
		renderTargetBlendDescription.DestBlendAlpha = D3D11_BLEND_ONE;
		renderTargetBlendDescription.BlendOpAlpha = D3D11_BLEND_OP_MAX;
		renderTargetBlendDescription.RenderTargetWriteMask = 15;

		D3D11_BLEND_DESC blendDescription{};
		blendDescription.RenderTarget[0] = renderTargetBlendDescription;

		HRESULT result = device->CreateBlendState(&blendDescription, &blend->blendState.p);
		assert(SUCCEEDED(result));
	}
	void GL::UpdateZTest(ZTest* zTest)
	{
		D3D11_DEPTH_STENCIL_DESC description{};
		description.DepthEnable = true;
		description.DepthWriteMask = zTest->write ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
		description.DepthFunc = (D3D11_COMPARISON_FUNC)zTest->operation;

		device->CreateDepthStencilState(&description, &zTest->depthStencilState.p);
	}


	// ���õ�ǰ��Ⱦ���������ڵ���ɫ��
	void GL::SetShader(ID3D11VertexShader* vertexShader, ID3D11InputLayout* vertexShaderInputLayout, ID3D11PixelShader* pixelShader)
	{
		context->IASetInputLayout(vertexShaderInputLayout);//�йض�����ɫ�������벼��
		context->VSSetShader(vertexShader, nullptr, 0);
		context->PSSetShader(pixelShader, nullptr, 0);
	}
	// ���õ�ǰ��Ⱦ�����еĶ�����������
	void GL::SetMesh(ObjectPtr<Mesh> mesh)
	{
		//�󶨶�������
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &mesh->vertexBuffer.p, &stride, &offset);

		//����������
		context->IASetIndexBuffer(mesh->triangleBuffer, DXGI_FORMAT_R32_UINT, 0);
	}
	// ���õ�ǰ��Ⱦ�����еĳ���������
	void GL::SetConstantBuffer(unsigned int startSlot, ID3D11Buffer** buffer)
	{
		context->VSSetConstantBuffers(startSlot, 1, buffer);
		context->PSSetConstantBuffers(startSlot, 1, buffer);
	}
	// ���õ�ǰ��Ⱦ�����е���ɫ����Դ
	void GL::SetTexture(unsigned int startSlot, ObjectPtr<Texture> texture)
	{
		ID3D11ShaderResourceView* resourceView = nullptr;
		if (texture != nullptr)
			resourceView = texture->GetResourceView().p;

		context->VSSetShaderResources(startSlot, 1, &resourceView);
		context->PSSetShaderResources(startSlot, 1, &resourceView);
	}
	// ������ɫ����������ʱ�ķ�ʽ
	void GL::SetSamplerState(ID3D11SamplerState** samplerState)
	{
		context->VSSetSamplers(0, 1, samplerState);
		context->PSSetSamplers(0, 1, samplerState);
	}
	// ������Ⱦ����Ŀ������
	void GL::SetRenderTarget(ObjectPtr<Texture2D> renderTexture) {
		GL::renderTexture = renderTexture;
		if (renderTexture != nullptr)
		{
			renderTargetView = renderTexture->renderTextureRTV;
			depthStencilView = renderTexture->depthTextureDSV;
			//�����ӿ����������������Сһ��
			D3D11_VIEWPORT viewport = {};
			viewport.Height = (float)renderTexture->GetHeight();
			viewport.Width = (float)renderTexture->GetWidth();
			viewport.MinDepth = 0;
			viewport.MaxDepth = 1;
			context->RSSetViewports(1, &viewport);
		}
		else//δ��ʱʹ��Ĭ��ֵ
		{
			renderTargetView = defaultRenderTargetView;
			depthStencilView = defaultDepthStencilView;
			//�����ӿ����������������Сһ��
			D3D11_VIEWPORT viewport = {};
			viewport.Height = (float)defaultRenderTargetDescription.Height;
			viewport.Width = (float)defaultRenderTargetDescription.Width;
			viewport.MinDepth = 0;
			viewport.MaxDepth = 1;
			context->RSSetViewports(1, &viewport);
		}

		//������ȾĿ��
		context->OMSetRenderTargets(1, &renderTargetView.p, depthStencilView);
	}
	void GL::SetRenderTarget(ObjectPtr<TextureCube> textureCube, int index)
	{
		if (textureCube != nullptr)
		{
			renderTargetView = textureCube->renderTextureRTV[index];
			depthStencilView = textureCube->depthTextureDSV;
			//�����ӿ����������������Сһ��
			D3D11_VIEWPORT viewport = {};
			viewport.Height = (float)textureCube->GetHeight();
			viewport.Width = (float)textureCube->GetWidth();
			viewport.MinDepth = 0;
			viewport.MaxDepth = 1;
			context->RSSetViewports(1, &viewport);
		}
		else//δ��ʱʹ��Ĭ��ֵ
		{
			renderTargetView = defaultRenderTargetView;
			depthStencilView = defaultDepthStencilView;
			//�����ӿ����������������Сһ��
			D3D11_VIEWPORT viewport = {};
			viewport.Height = (float)defaultRenderTargetDescription.Height;
			viewport.Width = (float)defaultRenderTargetDescription.Width;
			viewport.MinDepth = 0;
			viewport.MaxDepth = 1;
			context->RSSetViewports(1, &viewport);
		}

		//������ȾĿ��
		context->OMSetRenderTargets(1, &renderTargetView.p, depthStencilView);
	}

	void GL::SetBlend(Blend* blend)
	{
		//���û�Ϸ�ʽ
		context->OMSetBlendState(blend->blendState, nullptr, 0xffffffff);
	}
	void GL::SetZTest(ZTest* zTest)
	{
		context->OMSetDepthStencilState(zTest->depthStencilState, 0);
	}

	void GL::Clear(bool clearDepth, bool clearColor, Color backgroundColor, float depth)
	{
		//���û�����ͼ
		if (clearColor == true)
		{
			const float teal[] = { backgroundColor.r, backgroundColor.g,backgroundColor.b, backgroundColor.a };
			context->ClearRenderTargetView(renderTargetView, teal);
		}

		//�������ģ�������ͼ
		if (clearDepth)
		{
			context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, 0);
		}
	}
	void GL::Render(int indexsCount)
	{
		//��ʼ����
		context->DrawIndexed(indexsCount, 0, 0);
	}

	GL* GL::Initialize(Window* window)
	{
		GL::CreateDevice();
		GL::CreateSwapChain(window->GetHwnd());
		ResizeDefaultRenderTarget(window->GetScreenRect().GetSize());
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		return new GL();
	}

	CComPtr<ID3D11Device> GL::device = nullptr;
	CComPtr<ID3D11DeviceContext> GL::context = nullptr;
	CComPtr<IDXGISwapChain1> GL::swapChain = nullptr;
	D3D11_TEXTURE2D_DESC GL::defaultRenderTargetDescription = {};
	CComPtr<ID3D11RenderTargetView> GL::defaultRenderTargetView = nullptr;
	CComPtr<ID3D11DepthStencilView> GL::defaultDepthStencilView = nullptr;
	CComPtr<ID3D11RenderTargetView> GL::renderTargetView = nullptr;
	CComPtr<ID3D11DepthStencilView> GL::depthStencilView = nullptr;
	ObjectPtr<Texture2D> GL::renderTexture = nullptr;

	void GL::CreateDevice() {
		D3D_FEATURE_LEVEL levels[] = {
			D3D_FEATURE_LEVEL_11_1,
		};

		HRESULT result = D3D11CreateDevice(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			D3D11_CREATE_DEVICE_SINGLETHREADED
			| D3D11_CREATE_DEVICE_BGRA_SUPPORT
			| D3D11_CREATE_DEVICE_DEBUG,
			levels,
			ARRAYSIZE(levels),
			D3D11_SDK_VERSION,
			&device,
			NULL,
			&context
		);
		assert(SUCCEEDED(result));// Direct3D�豸�����Ƿ�ɹ�
	}
	void GL::CreateSwapChain(HWND hwnd)
	{
		//��ȡ�ײ�DXGI�Ĺ���
		CComPtr<IDXGIDevice> dxglDevice;
		CComPtr<IDXGIAdapter> dxglAdapter;
		CComPtr<IDXGIFactory2> dxglFactory;

		assert(SUCCEEDED(device->QueryInterface(&dxglDevice)));//��ȡDXGI�豸
		assert(SUCCEEDED(dxglDevice->GetAdapter(&dxglAdapter)));//��ȡDXGI������
		assert(SUCCEEDED(dxglAdapter->GetParent(IID_PPV_ARGS(&dxglFactory)))); //��ȡDXGI����

		UINT m4xMsaaQuality;
		device->CheckMultisampleQualityLevels(
			DXGI_FORMAT_B8G8R8A8_UNORM, 4, &m4xMsaaQuality);
		assert(m4xMsaaQuality > 0);


		//����������������
		DXGI_SWAP_CHAIN_DESC1 swapChainProperty = {};//�ֲ�����һ��Ҫ��ʼ��
		swapChainProperty.BufferCount = 2;
		swapChainProperty.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swapChainProperty.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainProperty.SampleDesc.Count = 1;
		swapChainProperty.SampleDesc.Quality = 0;
		swapChainProperty.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc = {};
		fullScreenDesc.Windowed = true;

		//����������
		assert(SUCCEEDED(dxglFactory->CreateSwapChainForHwnd(
			device,
			hwnd,
			&swapChainProperty,
			&fullScreenDesc,
			nullptr,
			&swapChain)
		));

	}
	void GL::CompileShader(const wchar_t* path, const char* entrypoint, const char* object, ID3DBlob** blob) {
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
			throw std::exception("������ɫ��ʧ��");
		}
	}

	CComPtr<ID3D11Texture2D> GL::GetDefaultRenderTarget() {
		CComPtr<ID3D11Texture2D> renderTargetTexture;
		swapChain->GetBuffer(0, IID_PPV_ARGS(&renderTargetTexture));
		return renderTargetTexture;
	}
	void GL::ResizeDefaultRenderTarget(Vector2 size)
	{
		HRESULT result = 0;

		//��Ҫ���������С������ͼһֱռ�������������û�����ã�����Ҫ���ͷ���ͼ
		if (renderTargetView == defaultRenderTargetView)renderTargetView = nullptr;
		if (depthStencilView == defaultDepthStencilView)depthStencilView = nullptr;
		defaultRenderTargetView = nullptr;
		defaultDepthStencilView = nullptr;

		//������Ⱦ�����С
		swapChain->ResizeBuffers(2, (UINT)size.x, (UINT)size.y, DXGI_FORMAT_B8G8R8A8_UNORM, 0);

		//���´�����ͼ

		//��ȡ��Ļ��ȾĿ�����������
		CComPtr<ID3D11Texture2D> targetTexture = nullptr;
		swapChain->GetBuffer(0, IID_PPV_ARGS(&targetTexture.p));
		targetTexture->GetDesc(&defaultRenderTargetDescription);
		//�Ըò��������������ģ����Ե�����
		CComPtr<ID3D11Texture2D> depthStencilTexture = nullptr;
		D3D11_TEXTURE2D_DESC depthStencilTextureDescription = {};
		depthStencilTextureDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilTextureDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilTextureDescription.Width = static_cast<UINT> (defaultRenderTargetDescription.Width);
		depthStencilTextureDescription.Height = static_cast<UINT> (defaultRenderTargetDescription.Height);
		depthStencilTextureDescription.ArraySize = 1;
		depthStencilTextureDescription.SampleDesc = { 1,0 };
		result = device->CreateTexture2D(&depthStencilTextureDescription, nullptr, &depthStencilTexture.p);
		assert(SUCCEEDED(result));
		//������ȾĿ��������ͼ
		CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{ D3D11_RTV_DIMENSION_TEXTURE2D };
		result = device->CreateRenderTargetView(targetTexture, &renderTargetViewDesc, &defaultRenderTargetView.p);
		assert(SUCCEEDED(result));
		//�������ģ����Ե���ͼ
		CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{ D3D11_DSV_DIMENSION_TEXTURE2D };
		result = device->CreateDepthStencilView(depthStencilTexture, &depthStencilViewDesc, &defaultDepthStencilView.p);
		assert(SUCCEEDED(result));
	}
	void GL::Present()
	{
		swapChain->Present(0, 0);
	}

}

