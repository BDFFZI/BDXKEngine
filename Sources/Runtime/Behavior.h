#pragma once
#include "Component.h"

namespace BDXKEngine {
	class Behavior :public Component
	{
	protected:
		void Awake()override;
		void Destroy()override;
	private:
	};
}


