#pragma once
#include "Input.h"

namespace BDXKEngine {
	class Cursor
	{
	public:
		static void SetLockState(bool value);
		static void SetVisible(bool value);
	protected:
		static Cursor* Initialize(Input* input, Window* window);
	private:
		static bool visible;
		static HCURSOR hCursor;
		static Window* window;
	};

}
