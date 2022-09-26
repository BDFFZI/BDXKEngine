#pragma once
#include <functional>
#include <vector>
#include "WindowBase.h"
#include "Base/Rect.h"

namespace BDXKEngine {
	enum class KeyCode :int {
		Backspace = 0x08,
		Delete = 0x2E,
		Tab = 0x09,
		Return = 0x0D,
		Esc = 0x1B,
		Space = 0x20,
		Alpha0 = 0x30,
		Alpha1 = 0x31,
		Alpha2 = 0x32,
		Alpha3 = 0x33,
		Alpha4 = 0x34,
		Alpha5 = 0x35,
		Alpha6 = 0x36,
		Alpha7 = 0x37,
		Alpha8 = 0x38,
		Alpha9 = 0x39,
		A = 0x41,
		B = 0x42,
		C = 0x43,
		D = 0x44,
		E = 0x45,
		F = 0x46,
		G = 0x47,
		H = 0x48,
		I = 0x49,
		J = 0x4A,
		K = 0x4B,
		L = 0x4C,
		M = 0x4D,
		N = 0x4E,
		O = 0x4F,
		P = 0x50,
		Q = 0x51,
		R = 0x52,
		S = 0x53,
		T = 0x54,
		U = 0x55,
		V = 0x56,
		W = 0x57,
		X = 0x58,
		Y = 0x59,
		Z = 0x5A,
		Shift = 0x10,
	};

	typedef std::function<void()> RenewEvent;
	typedef std::function<void(Vector2 size)> ResizeEvent;
	typedef std::function<void()> DestroyEvent;
	typedef std::function<void(Vector2 position)> MouseMoveEvent;
	typedef std::function<void(Vector2 delta)> MouseWheelEvent;
	typedef std::function<void(int button, bool state)> MouseButtonEvent;
	typedef std::function<void(KeyCode code, bool state)> KeyCodeEvent;
	typedef std::function<void(char character)> CharacterEvent;

	class Window final : public WindowBase
	{
	public:
		Window(const wchar_t* name);

		void AddRenewEvent(RenewEvent renewEvent);
		void AddResizeEvent(ResizeEvent resizeEvent);
		void AddDestroyEvent(DestroyEvent destroyEvent);
		void AddMouseMoveEvent(MouseMoveEvent mouseMoveEvent);
		void AddMouseWheelEvent(MouseWheelEvent mouseWheelEvent);
		void AddMouseButtonEvent(MouseButtonEvent mouseButtonEvent);
		void AddKeyCodeEvent(KeyCodeEvent keyCodeEvent);
		void AddCharacterEvent(CharacterEvent characterEvent);

		Rect GetScreenRect();
		Vector2 GetSize();
		Vector2 GetCursorLocalPosition();
		Vector2 GetCursorMoveDelta();

		void SetCursorTrack(bool state);
		void SetCursorConfining(bool state);
		void SetCursorLocalPosition(Vector2 localPosition);
		void SetCursorLock(bool state);
		void SetCursorVisible(bool state);

	protected:
		std::vector<RenewEvent> renewEvents;
		std::vector<ResizeEvent> resizeEvents;
		std::vector<DestroyEvent> destroyEvents;
		std::vector<MouseMoveEvent> mouseMoveEvents;
		std::vector<MouseWheelEvent> mouseWheelEvents;
		std::vector<MouseButtonEvent> mouseButtonEvents;
		std::vector<KeyCodeEvent> keyCodeEvents;
		std::vector<CharacterEvent> characterEvents;

		Vector2 lockCursorPos = {};
		Vector2 lastCursorPos = {};
		Vector2 cursorPos = {};
		bool cursorlock = false;
		bool cursorVisible = true;
		HCURSOR hCursor = LoadCursor(nullptr, IDC_ARROW);

		void UpdateCursor();

		LRESULT HandleMessage(UINT messageSign, WPARAM wparameter, LPARAM lparameter) override;
	};
}
