#include "GL.h"
#include"Assert.h"
#include"Debug.h"

CComPtr<ID3D11Device> GL::device;
CComPtr<ID3D11DeviceContext> GL::context;
CComPtr<IDXGISwapChain> GL::swapChain;


CComPtr<ID3D11RenderTargetView> GL::renderTargetView;
CComPtr<ID3D11DepthStencilView> GL::depthStencilView;


void GL::CreateDevice() {
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_11_0,
	};

	HRESULT result = D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
		levels,
		ARRAYSIZE(levels),
		D3D11_SDK_VERSION,
		&device,
		NULL,
		&context
	);
	Assert::IsSucceeded(result, L"Direct3D�豸����ʧ��");
}
void GL::CreateSwapChain(HWND hwnd)
{
	//��ȡ�ײ�DXGI�Ĺ���
	CComPtr<IDXGIDevice> dxglDevice;
	CComPtr<IDXGIAdapter> dxglAdapter;
	CComPtr<IDXGIFactory> dxglFactory;

	Assert::IsSucceeded(device->QueryInterface(&dxglDevice), L"DXGI�豸��ȡʧ��");
	Assert::IsSucceeded(dxglDevice->GetAdapter(&dxglAdapter), L"DXGI��������ȡʧ��");
	Assert::IsSucceeded(dxglAdapter->GetParent(IID_PPV_ARGS(&dxglFactory)), L"DXGI������ȡʧ��");

	//����������������
	DXGI_SWAP_CHAIN_DESC swapChainProperty = {};//�ֲ�����һ��Ҫ��ʼ��
	swapChainProperty.Windowed = TRUE;
	swapChainProperty.OutputWindow = hwnd;
	swapChainProperty.BufferCount = 2;
	swapChainProperty.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainProperty.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainProperty.SampleDesc.Count = 1;
	swapChainProperty.SampleDesc.Quality = 0;
	swapChainProperty.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	//����������
	HRESULT result = dxglFactory->CreateSwapChain(device, &swapChainProperty, &swapChain);
	Assert::IsSucceeded(result, L"Direct3D����������ʧ��");
}
void GL::CreateRenderTexture()
{
	CComPtr<ID3D11Texture2D> colorTexture;
	CComPtr<ID3D11Texture2D> depthStencilTexture;

	//��ȡ��ɫ��������������Ⱦ��ͼΪ��ɫ������
	swapChain->GetBuffer(0, IID_PPV_ARGS(&colorTexture));
	Assert::IsSucceeded(
		device->CreateRenderTargetView(colorTexture, nullptr, &renderTargetView),
		L"������Ⱦ��ͼʧ��"
	);
	//��ȡ��ɫ����������
	CD3D11_TEXTURE2D_DESC colorTextureDescription;
	colorTexture->GetDesc(&colorTextureDescription);
	//�Ըò����������ģ�建����
	CD3D11_TEXTURE2D_DESC depthStencilTextureDescription(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		static_cast<UINT> (colorTextureDescription.Width),
		static_cast<UINT> (colorTextureDescription.Height),
		1, // This depth stencil view has only one texture.
		1, // Use a single mipmap level.
		D3D11_BIND_DEPTH_STENCIL
	);
	Assert::IsSucceeded(
		device->CreateTexture2D(&depthStencilTextureDescription, nullptr, &depthStencilTexture),
		L"������Ȳ�������ʧ��"
	);
	//��������Ϊ���ģ����Ե���ͼ
	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	Assert::IsSucceeded(
		device->CreateDepthStencilView(depthStencilTexture, &depthStencilViewDesc, &depthStencilView),
		L"������Ȳ�����ͼʧ��"
	);

	D3D11_VIEWPORT viewport = {};
	viewport.Height = (float)colorTextureDescription.Height;
	viewport.Width = (float)colorTextureDescription.Width;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;

	context->RSSetViewports(1, &viewport);
}

void GL::CreateVertexShader(const char* path, D3D11_INPUT_ELEMENT_DESC inputLayoutDescriptor[], int inputLayoutDescriptorCount, ID3D11VertexShader** vertexShader, ID3D11InputLayout** inputLayout)
{
	const int byteBufferSize = 32 * 1024;
	//��ȡ������ɫ�������ƴ���
	FILE* vertexShaderFile;
	fopen_s(&vertexShaderFile, path, "rb");
	Assert::IsTrue(vertexShaderFile != nullptr, L"��ȡ������ɫ���ļ�ʧ��");
	if (vertexShaderFile == 0)
		throw "Ϊ�������༭������";

	char* vertexShaderBytes = new char[byteBufferSize];
	size_t vertexShaderBytesSize = fread_s(vertexShaderBytes, byteBufferSize, 1, byteBufferSize, vertexShaderFile);
	fclose(vertexShaderFile);
	//����������ɫ��
	Assert::IsSucceeded(device->CreateVertexShader(
		vertexShaderBytes,
		vertexShaderBytesSize,
		nullptr,
		vertexShader
	), L"����������ɫ��ʧ��");

	//����������Ϣ
	Assert::IsSucceeded(device->CreateInputLayout(
		inputLayoutDescriptor,
		inputLayoutDescriptorCount,
		vertexShaderBytes,
		vertexShaderBytesSize,
		inputLayout
	), L"����������Ϣʧ��");

	delete[] vertexShaderBytes;
}
void GL::CreatePixelShader(const char* path, ID3D11PixelShader** pixelShader)
{
	const int byteBufferSize = 32 * 1024;
	//��ȡ������ɫ�������ƴ���
	FILE* pixelShaderFile;
	fopen_s(&pixelShaderFile, "C:/Users/BDFFZI/Desktop/BDXKEngine/Shader/PixelShader.cso", "rb");
	Assert::IsTrue(pixelShaderFile != nullptr, L"��ȡ������ɫ���ļ�ʧ��");
	if (pixelShaderFile == 0)
		throw "Ϊ�������༭������";

	char* pixelShaderBytes = new char[byteBufferSize];
	size_t pixelShaderBytesSize = fread_s(pixelShaderBytes, byteBufferSize, 1, byteBufferSize, pixelShaderFile);
	fclose(pixelShaderFile);
	//����������ɫ��
	Assert::IsSucceeded(device->CreatePixelShader(
		pixelShaderBytes,
		pixelShaderBytesSize,
		nullptr,
		pixelShader
	), L"����������ɫ��ʧ��");

	delete[] pixelShaderBytes;
}

void GL::Clear(Color color)
{
	const float teal[] = { color.r, color.g,color.b, color.a };
	context->ClearRenderTargetView(
		renderTargetView,
		teal
	);
}
void GL::Render(
	ID3D11Buffer* vertexBuffer, ID3D11InputLayout* inputLayout, int vertexSize,
	ID3D11Buffer* indexBuffer, DXGI_FORMAT indexFormat, int indexsCount,
	ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader,
	D3D11_PRIMITIVE_TOPOLOGY drawMode)
{
	//�󶨶�������
	UINT stride = vertexSize;
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	//�󶨶�����Ϣ
	context->IASetInputLayout(inputLayout);

	//����������
	context->IASetIndexBuffer(indexBuffer, indexFormat, 0);

	//����ɫ��
	context->VSSetShader(vertexShader, nullptr, 0);
	context->PSSetShader(pixelShader, nullptr, 0);

	//���û���Ŀ��
	context->OMSetRenderTargets(1, &(renderTargetView.p), NULL);

	//���û���ģʽ
	context->IASetPrimitiveTopology(drawMode);

	//��ʼ����
	context->DrawIndexed(indexsCount, 0, 0);

	//��ʾ���
	swapChain->Present(0, 0);

	//D3D11_VIEWPORT viewport;
	//ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	//viewport.TopLeftX = 0;
	//viewport.TopLeftY = 0;
	//viewport.Width = 800;
	//viewport.Height = 600;
	//context->RSSetViewports(1, &viewport);
}

