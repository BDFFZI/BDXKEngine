#include "GL.h"
#include <d3dcompiler.h>
#include <exception>

namespace BDXKEngine {

	void GL::CreateVertexShader(const wchar_t* path, D3D11_INPUT_ELEMENT_DESC inputLayoutDescriptor[], int inputLayoutDescriptorCount,
		ID3D11VertexShader** vertexShader, ID3D11InputLayout** inputLayout)
	{
		CComPtr<ID3DBlob> blob;
		CompileShader(path, "main", "vs_5_0", &blob.p);
		//����������ɫ��
		assert(SUCCEEDED(device->CreateVertexShader(
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			nullptr,
			vertexShader
		)));

		//����������Ϣ
		assert(SUCCEEDED(device->CreateInputLayout(
			inputLayoutDescriptor,
			inputLayoutDescriptorCount,
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			inputLayout
		)));
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

	void GL::CreateTexture2D(unsigned int width, unsigned int height, ID3D11Texture2D** texture, ID3D11ShaderResourceView** resourceView)
	{
		D3D11_TEXTURE2D_DESC textureDescription = CD3D11_TEXTURE2D_DESC(
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			width,
			height);
		textureDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

		assert(SUCCEEDED(device->CreateTexture2D(&textureDescription, nullptr, texture)));

		D3D11_SHADER_RESOURCE_VIEW_DESC resourceDescription{};
		resourceDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		resourceDescription.Format = textureDescription.Format;
		resourceDescription.Texture2D = D3D11_TEX2D_SRV{};

		assert(SUCCEEDED(device->CreateShaderResourceView(*texture, nullptr, resourceView)));
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
	void GL::SetBuffer(ID3D11Buffer* vertexBuffer, int vertexSize, ID3D11Buffer* indexBuffer, DXGI_FORMAT indexFormat)
	{
		//�󶨶�������
		UINT stride = vertexSize;
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

		//����������
		context->IASetIndexBuffer(indexBuffer, indexFormat, 0);
	}
	// ���õ�ǰ��Ⱦ�����еĳ���������
	void GL::SetConstantBuffer(unsigned int startSlot, ID3D11Buffer** buffer)
	{
		context->VSSetConstantBuffers(startSlot, 1, buffer);
		context->PSSetConstantBuffers(startSlot, 1, buffer);
	}
	// ���õ�ǰ��Ⱦ�����е���ɫ����Դ
	void GL::SetShaderResource(unsigned int startSlot, ID3D11ShaderResourceView** resource)
	{
		context->VSSetShaderResources(startSlot, 1, resource);
		context->PSSetShaderResources(startSlot, 1, resource);
	}
	// ������ɫ����������ʱ�ķ�ʽ
	void GL::SetSamplerState(ID3D11SamplerState** samplerState)
	{
		context->VSSetSamplers(0, 1, samplerState);
		context->PSSetSamplers(0, 1, samplerState);
	}
	// ������Ⱦ����Ŀ������
	void GL::SetRenderTexture(ID3D11Texture2D* renderTargetTexture) {
		CComPtr<ID3D11Texture2D> defaultTexture;
		if (renderTargetTexture == nullptr)//δ��ʱʹ��Ĭ����Ⱦ����
		{
			swapChain->GetBuffer(0, IID_PPV_ARGS(&defaultTexture.p));
			renderTargetTexture = defaultTexture.p;
		}

		renderTargetView = nullptr;
		depthStencilView = nullptr;

		HRESULT result = 0;

		//��ȡ��Ⱦ�������
		CD3D11_TEXTURE2D_DESC colorTextureDescription;
		renderTargetTexture->GetDesc(&colorTextureDescription);
		//�Ըò��������������ģ����Ե��������
		CD3D11_TEXTURE2D_DESC depthStencilTextureDescription(
			DXGI_FORMAT_D24_UNORM_S8_UINT,
			static_cast<UINT> (colorTextureDescription.Width),
			static_cast<UINT> (colorTextureDescription.Height),
			1,
			0,
			D3D11_BIND_DEPTH_STENCIL
		);

		//������Ȳ�������
		CComPtr<ID3D11Texture2D> depthStencilTexture;
		assert(SUCCEEDED(device->CreateTexture2D(&depthStencilTextureDescription, nullptr, &depthStencilTexture.p)));

		//������ȾĿ��������ͼ
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
		renderTargetViewDesc.Format = colorTextureDescription.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;
		result = device->CreateRenderTargetView(renderTargetTexture, &renderTargetViewDesc, &renderTargetView.p);
		assert(SUCCEEDED(result));
		//�������ģ����Ե���ͼ
		CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
		assert(SUCCEEDED(device->CreateDepthStencilView(depthStencilTexture, &depthStencilViewDesc, &depthStencilView.p)));

		//�����ӿ�������������ͼ��Сһ��
		D3D11_VIEWPORT viewport = {};
		viewport.Height = (float)colorTextureDescription.Height;
		viewport.Width = (float)colorTextureDescription.Width;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;
		context->RSSetViewports(1, &viewport);
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

	void GL::Viewport(Rect rect)
	{
		//��ǰ�ͷţ���Ȼ����Ϊʹ���ж��޷����������С
		renderTargetView = nullptr;
		depthStencilView = nullptr;

		//������Ⱦ�����С
		swapChain->ResizeBuffers(2, (UINT)rect.width, (UINT)rect.height, DXGI_FORMAT_B8G8R8A8_UNORM, 0);

		SetRenderTexture(NULL);
	}
	void GL::Begin(D3D11_PRIMITIVE_TOPOLOGY drawMode)
	{
		//����ɫ��������ģ������õ���ͼ(Present�ᵼ�½���󶨣�����ÿ�ζ����°�)
		context->OMSetRenderTargets(1, &renderTargetView.p, depthStencilView);

		//���û���ģʽ
		context->IASetPrimitiveTopology(drawMode);
	}
	void GL::End()
	{
		//��ʾ���
		swapChain->Present(0, 0);
	}
	void GL::Clear(bool clearDepth, bool clearColor, Color backgroundColor, float depth)
	{
		//���û�����ͼ
		if (clearColor == true)
		{
			const float teal[] = { backgroundColor.r, backgroundColor.g,backgroundColor.b, backgroundColor.a };
			context->ClearRenderTargetView(
				renderTargetView,
				teal
			);
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

	CComPtr<ID3D11Texture2D> GL::GetRenderTargetTexture() {
		CComPtr<ID3D11Texture2D> renderTargetTexture;
		swapChain->GetBuffer(0, IID_PPV_ARGS(&renderTargetTexture));
		return renderTargetTexture;
	}

	GL* GL::Initialize(HWND window)
	{
		GL::CreateDevice();
		GL::CreateSwapChain(window);
		SetRenderTexture(NULL);
		return new GL();
	}

	CComPtr<ID3D11Device> GL::device;
	CComPtr<ID3D11DeviceContext> GL::context;
	CComPtr<IDXGISwapChain1> GL::swapChain;

	CComPtr<ID3D11RenderTargetView> GL::renderTargetView;
	CComPtr<ID3D11DepthStencilView> GL::depthStencilView;

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
		swapChainProperty.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

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
	void GL::CompileShader(const wchar_t* path, const char* entrypoint, const char* target, ID3DBlob** blob) {
		CComPtr<ID3DBlob> compileError;
		HRESULT result = D3DCompileFromFile(path,
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entrypoint,
			target,
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
}

