#pragma once
#include <BDXKEngine.h>

namespace BDXKEditor {
	using namespace BDXKEngine;
	class MaterialHUD :public Component, public PostRenderHandler {
	public:
		std::vector<ObjectPtr<Material>> materials;
		float metallic = 0;
		float smoothness = 0;

	private:
		void OnPostRender()override {
			Vector2 windowSize = Screen::GetSize();
			Rect rect = {
				windowSize.x - 100,
				windowSize.y - 90,
				90,
				20
			};


			smoothness = GUI::Slider(rect, smoothness, 0, 1);
			rect.y -= 30;
			metallic = GUI::Slider(rect, metallic, 0, 1);

			std::for_each(
				materials.begin(),
				materials.end(),
				[&](ObjectPtr<Material> material)
				{
					material->SetFloat(0, metallic);
					material->SetFloat(1, smoothness);
				}
			);
		}
	};
}
