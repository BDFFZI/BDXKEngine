#pragma once
#include <functional>
#include "GL.h"
#include "GL2D.h"
#include "Mesh.h"
#include "Material.h"
#include "Window.h"
#include "Texture2D.h"
#include "RenderEvent.h"

namespace BDXKEngine {
	class Graphics :protected GL, protected GL2D, MeshEditor, ShaderEditor, Texture2DEditor, RenderEventEditor
	{
	public:
		/// <summary>
		/// Ĭ�������HLSL����������˳��ʽ�������ݣ������������������������
		/// ��ᵼ����ɫ���õ��ľ���ᱻת�ã����Ǿͳ�������������
		/// </summary>
		/// <param name="matrix"></param>
		static void UpdateWorldInfo(WorldInfo worldInfo);
		static void UpdateLightInfo(LightInfo lightInfo);

		static void DrawMeshNow(Mesh* mesh);
		static void SetRenderTarget(Texture2D* texture);
		static void Flush();
	protected:
		static Graphics* Initialize(Window* window, GL** gl, GL2D** gl2d);
	private:
		static WorldInfo worldInfo;
		static LightInfo lightInfo;

		static CComPtr<ID3D11Buffer> worldInfoBuffer;
		static CComPtr<ID3D11Buffer> lightInfoBuffer;

		static void OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter);
		static CComPtr<IDXGISurface> GetIDXGISurface();
	};
}


