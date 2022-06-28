#include "GL.h"

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
		DXGI_FORMAT_B8G8R8A8_UNORM, 4, &m4xMsaaQuality);    // ע��˴�DXGI_FORMAT_B8G8R8A8_UNORM
	assert(m4xMsaaQuality > 0);


	//����������������
	DXGI_SWAP_CHAIN_DESC1 swapChainProperty = {};//�ֲ�����һ��Ҫ��ʼ��
	swapChainProperty.BufferCount = 2;
	swapChainProperty.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainProperty.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainProperty.SampleDesc.Count = 1;
	swapChainProperty.SampleDesc.Quality = 0;
	swapChainProperty.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc{};
	fullScreenDesc.Windowed = true;

	//����������
	assert(SUCCEEDED(dxglFactory->CreateSwapChainForHwnd(device, hwnd, &swapChainProperty, &fullScreenDesc, nullptr, &swapChain)));

}
void GL::CreateRenderTexture()
{
	//������Ĭ�ϴ�����Ⱦ������ȡ�����
	CComPtr<ID3D11Texture2D> renderTargetTexture;
	swapChain->GetBuffer(0, IID_PPV_ARGS(&renderTargetTexture.p));
	CD3D11_TEXTURE2D_DESC colorTextureDescription;
	renderTargetTexture->GetDesc(&colorTextureDescription);
	renderTargetTexture = nullptr;

	Viewport({ {0,0},{(float)colorTextureDescription.Width,(float)colorTextureDescription.Height } });
}

void GL::CreateVertexShader(const char* path, D3D11_INPUT_ELEMENT_DESC inputLayoutDescriptor[], int inputLayoutDescriptorCount, 
	ID3D11VertexShader** vertexShader, ID3D11InputLayout** inputLayout)
{
	const int byteBufferSize = 32 * 1024;
	//��ȡ������ɫ�������ƴ���
	FILE* vertexShaderFile;
	fopen_s(&vertexShaderFile, path, "rb");
	if (vertexShaderFile == 0)
		throw "������ɫ���ļ���ʧ��";

	char* vertexShaderBytes = new char[byteBufferSize];
	size_t vertexShaderBytesSize = fread_s(vertexShaderBytes, byteBufferSize, 1, byteBufferSize, vertexShaderFile);
	fclose(vertexShaderFile);
	//����������ɫ��
	assert(SUCCEEDED(device->CreateVertexShader(
		vertexShaderBytes,
		vertexShaderBytesSize,
		nullptr,
		vertexShader
	)));

	//����������Ϣ
	assert(SUCCEEDED(device->CreateInputLayout(
		inputLayoutDescriptor,
		inputLayoutDescriptorCount,
		vertexShaderBytes,
		vertexShaderBytesSize,
		inputLayout
	)));

	delete[] vertexShaderBytes;
}
void GL::CreatePixelShader(const char* path, ID3D11PixelShader** pixelShader)
{
	const int byteBufferSize = 32 * 1024;
	//��ȡ������ɫ�������ƴ���
	FILE* pixelShaderFile;
	fopen_s(&pixelShaderFile, path, "rb");
	if (pixelShaderFile == 0)
		throw "������ɫ���ļ���ʧ��";

	char* pixelShaderBytes = new char[byteBufferSize];
	size_t pixelShaderBytesSize = fread_s(pixelShaderBytes, byteBufferSize, 1, byteBufferSize, pixelShaderFile);
	fclose(pixelShaderFile);

	//����������ɫ��
	assert(SUCCEEDED(device->CreatePixelShader(
		pixelShaderBytes,
		pixelShaderBytesSize,
		nullptr,
		pixelShader
	)));

	delete[] pixelShaderBytes;
}

void GL::Viewport(Rect rect)
{
	renderTargetView = nullptr;
	depthStencilView = nullptr;

	//������Ⱦ�����С
	swapChain->ResizeBuffers(2, (UINT)rect.width, (UINT)rect.height, DXGI_FORMAT_B8G8R8A8_UNORM, 0);
	//��ȡ��Ⱦ���������
	CComPtr<ID3D11Texture2D> renderTargetTexture;
	swapChain->GetBuffer(0, IID_PPV_ARGS(&renderTargetTexture.p));
	CD3D11_TEXTURE2D_DESC colorTextureDescription;
	renderTargetTexture->GetDesc(&colorTextureDescription);

	//������ȾĿ��������ͼ
	assert(SUCCEEDED(device->CreateRenderTargetView(renderTargetTexture, nullptr, &renderTargetView)));

	//�Ըò��������������ģ����Ե�����
	CD3D11_TEXTURE2D_DESC depthStencilTextureDescription(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		static_cast<UINT> (colorTextureDescription.Width),
		static_cast<UINT> (colorTextureDescription.Height),
		1, // This depth stencil view has only one texture.
		1, // Use a single mipmap level.
		D3D11_BIND_DEPTH_STENCIL
	);
	//������Ȳ�������
	CComPtr<ID3D11Texture2D> depthStencilTexture;
	assert(SUCCEEDED(device->CreateTexture2D(&depthStencilTextureDescription, nullptr, &depthStencilTexture.p)));
	//�������ģ����Ե���ͼ
	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	assert(SUCCEEDED(device->CreateDepthStencilView(depthStencilTexture, &depthStencilViewDesc, &depthStencilView)));

	//���°����´�������ͼ
	context->OMSetRenderTargets(1, &renderTargetView.p, depthStencilView);

	//�����ӿ�����
	D3D11_VIEWPORT viewport = {};
	viewport.Height = (float)colorTextureDescription.Height;
	viewport.Width = (float)colorTextureDescription.Width;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	context->RSSetViewports(1, &viewport);
}

void GL::Begin()
{
	//����ɫ��������ģ������õ���ͼ(Present�ᵼ�½���󶨣�����ÿ�ζ����°�)
	context->OMSetRenderTargets(1, &renderTargetView.p, depthStencilView);
}

void GL::End()
{
	//��ʾ���
	swapChain->Present(0, 0);
}

void GL::Clear(Color color, float depth, unsigned char stencil)
{
	const float teal[] = { color.r, color.g,color.b, color.a };
	//���û�����ͼ
	context->ClearRenderTargetView(
		renderTargetView,
		teal
	);
	//�������ģ�������ͼ
	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
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

	//���û���ģʽ
	context->IASetPrimitiveTopology(drawMode);

	//��ʼ����
	context->DrawIndexed(indexsCount, 0, 0);
}

