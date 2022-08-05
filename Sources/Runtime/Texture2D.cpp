#include "Texture2D.h"
#include "GL.h"

namespace BDXKEngine {
	Texture2D::Texture2D(unsigned int width, unsigned int height) : Texture2D()
	{
		HRESULT result;

		//������Ⱦ����
		renderTextureDesc = D3D11_TEXTURE2D_DESC{};
		renderTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		renderTextureDesc.Width = width;
		renderTextureDesc.Height = height;
		renderTextureDesc.ArraySize = 1;
		renderTextureDesc.MipLevels = 1;
		renderTextureDesc.SampleDesc = { 1,0 };
		renderTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		result = device->CreateTexture2D(&renderTextureDesc, nullptr, &renderTexture.p);
		assert(SUCCEEDED(result));
		//������ȾĿ����ͼ��ʹ�����Ϊ����������Ⱦ���
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		result = device->CreateRenderTargetView(renderTexture, &renderTargetViewDesc, &renderTextureRTV.p);
		assert(SUCCEEDED(result));
		//������Ⱦ������ɫ����Դ��ͼ��ʹ�����Ϊ��ͼʹ��
		result = device->CreateShaderResourceView(renderTexture, nullptr, &renderTextureSRV.p);
		assert(SUCCEEDED(result));

		//�����������
		CreateDepthStencil(width, height, &depthTexture.p, &depthTextureDSV.p);

		//����������
		CreateSamplerState(&samplerState.p);
	}
	Texture2D::Texture2D(Color color) :Texture2D(1, 1)
	{
		ObjectPtr<Texture2D> renderTarget = this;
		renderTarget.SuppressDestroy();
		GL::SetRenderTarget(renderTarget);
		GL::Clear(true, true, color);
		GL::SetRenderTarget(nullptr);
	}
	Texture2D::Texture2D(char* filePath) :Texture2D() {

	}

	unsigned int Texture2D::GetWidth()
	{
		return renderTextureDesc.Width;
	}
	unsigned int Texture2D::GetHeight()
	{
		return renderTextureDesc.Height;
	}

	Texture2D::Texture2D() : Texture(L"New Texture2D")
	{

	}

	CComPtr<ID3D11ShaderResourceView> Texture2D::GetResourceView()
	{
		return renderTextureSRV;
	}
	CComPtr<ID3D11SamplerState> Texture2D::GetSamplerState()
	{
		return samplerState;
	}
}
