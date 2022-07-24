#pragma once
#include "Window.h"
#include "GameObject.h"

namespace BDXKEngine {
	class GameObjectManager {
	protected:
		static GameObjectManager* Initialize(Window* window);
	private:
		static void OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter);
	};
}