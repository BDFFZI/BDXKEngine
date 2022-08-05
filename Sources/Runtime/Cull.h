#pragma once
#include<d3d11.h>

namespace BDXKEngine {
	struct Cull {
		enum class State {
			Back,
			Front,
			Off,
		};

		State state = State::Back;
	};
}

