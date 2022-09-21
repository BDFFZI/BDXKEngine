#include "Resources.h"
#include "Platform/IO/MeshImporter.h"

namespace BDXKEngine {
	ObjectPtr<Material> Resources::shadowMapMaterial = nullptr;
	ObjectPtr<Material> Resources::skyboxMaterial = nullptr;
	ObjectPtr<Material> Resources::blitMaterial = nullptr;
	ObjectPtr<Texture2D> Resources::whiteTexture = nullptr;
	ObjectPtr<Mesh> Resources::cubeMesh = nullptr;
	ObjectPtr<Mesh> Resources::sphereMesh = nullptr;
	ObjectPtr<Mesh> Resources::blenderMesh = nullptr;

	ObjectPtr<Material> Resources::GetShadowMapMaterial()
	{
		return shadowMapMaterial;
	}
	ObjectPtr<Material> Resources::GetSkyboxMaterial()
	{
		return skyboxMaterial;
	}
	ObjectPtr<Material> Resources::GetBlitMaterial()
	{
		return blitMaterial;
	}
	ObjectPtr<Texture2D> Resources::GetWhiteTexture()
	{
		return whiteTexture;
	}
	ObjectPtr<Mesh> Resources::GetCubeMesh()
	{
		return cubeMesh;
	}
	ObjectPtr<Mesh> Resources::GetSphereMesh()
	{
		return sphereMesh;
	}
	ObjectPtr<Mesh> Resources::GetBlenderMesh()
	{
		return blenderMesh;
	}

	void BDXKEngine::Resources::Initialize(Window* window)
	{
		//绘制阴影贴图
		shadowMapMaterial = Material::Create({ Shader::Create(
			GetResourcesPathW(Shaders, ShadowMap\\VertexShader.hlsl),
			GetResourcesPathW(Shaders, ShadowMap\\PixelShader.hlsl),
			PassType::ForwardBase
		) });
		//绘制天空盒
		skyboxMaterial = Material::Create({ Shader::Create(
			GetResourcesPathW(Shaders, Skybox\\VertexShader.hlsl),
			GetResourcesPathW(Shaders, Skybox\\PixelShader.hlsl),
			PassType::ForwardBase
		) });
		for (ObjectPtr<Shader> shader : skyboxMaterial->GetShaders())
		{
			ZTest ztest = {};
			ztest.write = false;
			ztest.operation = ZTest::Operation::Always;
			shader->SetZTest(ztest);
		}
		//位块传输
		blitMaterial = Material::Create({
			Shader::Create(
				GetResourcesPathW(Shaders, Blit\\VertexShader.hlsl),
				GetResourcesPathW(Shaders, Blit\\PixelShader.hlsl),
				PassType::ForwardBase
			)
			});
		//白色贴图
		whiteTexture = Texture2D::Create(Color::white);

		cubeMesh = MeshImporter::Import((char*)GetResourcesPath(Meshes, Cube.glb)).CreateMesh();
		sphereMesh = MeshImporter::Import((char*)GetResourcesPath(Meshes, Sphere.glb)).CreateMesh();
		blenderMesh = MeshImporter::Import((char*)GetResourcesPath(Meshes, Blender.glb)).CreateMesh();

		window->AddDestroyEvent([]() {
			shadowMapMaterial = nullptr;
			skyboxMaterial = nullptr;
			blitMaterial = nullptr;
			whiteTexture = nullptr;
			cubeMesh = nullptr;
			sphereMesh = nullptr;
			blenderMesh = nullptr;
			});
	}
}