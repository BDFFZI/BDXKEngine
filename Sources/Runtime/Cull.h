#pragma once
#include <d3d11.h>
#include <atlbase.h>

namespace BDXKEngine {
	class GL;
	struct Cull {
		friend GL;

		enum class State {
			Back,
			Front,
			Off,
		};

		State state = State::Back;
	};
}

