#include "Manager.h"

BDXKEngine::Manager::Manager(Window* window)
{
	window->AddMessageListener([=](Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter) {
		OnWindowMessage(window, messageSign, wparameter, lparameter);
	});
}
