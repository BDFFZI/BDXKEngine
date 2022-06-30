#include "Input.h"

Window* Input::window{};
float Input::mouseScrollDelta;
Vector2 Input::mousePosition;
bool Input::mouseButtonState[3][3];
bool Input::keyboardState[256][3];
bool Input::enable{ true };

bool Input::GetMouseButtonDown(int mouseButtonIndex)
{
	if (mouseButtonState[mouseButtonIndex][0] == false &&
		mouseButtonState[mouseButtonIndex][1] == true)
		return true;
	return false;
}
bool Input::GetMouseButton(int mouseButtonIndex)
{
	if (mouseButtonState[mouseButtonIndex][0] == true &&
		mouseButtonState[mouseButtonIndex][1] == true)
		return true;
	return false;
}
bool Input::GetMouseButtonUp(int mouseButtonIndex)
{
	if (mouseButtonState[mouseButtonIndex][0] == true &&
		mouseButtonState[mouseButtonIndex][1] == false)
		return true;
	return false;
}

bool Input::GetKeyDown(KeyCode keyCode)
{
	if (keyboardState[(int)keyCode][0] == false &&
		keyboardState[(int)keyCode][1] == true)
		return true;
	return false;
}
bool Input::GetKey(KeyCode keyCode)
{
	if (keyboardState[(int)keyCode][0] == true &&
		keyboardState[(int)keyCode][1] == true)
		return true;
	return false;
}
bool Input::GetKeyUp(KeyCode keyCode)
{
	if (keyboardState[(int)keyCode][0] == true &&
		keyboardState[(int)keyCode][1] == false)
		return true;
	return false;
}

Input* Input::Initialize(Window* window)
{
	Input::window = window;
	mousePosition = window->GetCursorLocalPosition();
	window->AddMessageListener(OnWindowMessage);
	return new Input{};
}

void Input::SetEnable(bool state)
{
	enable = state;
}

void Input::FlushState()
{
	mouseScrollDelta *= 0.7f;

	for (int i = 0; i < 3; i++)
	{
		mouseButtonState[i][0] = mouseButtonState[i][1];
		mouseButtonState[i][1] = mouseButtonState[i][2];
	}
	for (int i = 0; i < 256; i++)
	{
		keyboardState[i][0] = keyboardState[i][1];
		keyboardState[i][1] = keyboardState[i][2];
	}
}
void Input::OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)
{
	if (messageSign == WM_PAINT)
	{
		Input::FlushState();
	}
	else if (enable)
	{
		switch (messageSign)
		{
#pragma region 鼠标事件
		case WM_MOUSEMOVE:
		{
			Input::mousePosition.x = (float)(lparameter << 48 >> 48);
			Input::mousePosition.y = (float)(lparameter >> 16);
			break;
		}
		case WM_MOUSEWHEEL:
		{
			Input::mouseScrollDelta += GET_WHEEL_DELTA_WPARAM(wparameter) / 1000.0f;
			break;
		}
		case WM_LBUTTONDOWN:
		{
			Input::mouseButtonState[0][2] = true;
			break;
		}
		case WM_LBUTTONUP:
		{
			Input::mouseButtonState[0][2] = false;
			break;
		}
		case WM_RBUTTONDOWN:
		{
			Input::mouseButtonState[1][2] = true;
			break;
		}
		case WM_RBUTTONUP:
		{
			Input::mouseButtonState[1][2] = false;
			break;
		}
		case WM_MBUTTONDOWN:
		{
			Input::mouseButtonState[2][2] = true;
			break;
		}
		case WM_MBUTTONUP:
		{
			Input::mouseButtonState[2][2] = false;
			break;
		}
#pragma endregion
#pragma region 键盘事件
		case WM_KEYDOWN:
		{
			Input::keyboardState[wparameter][2] = true;
			break;
		}
		case WM_KEYUP:
		{
			Input::keyboardState[wparameter][2] = false;
			break;
		}
#pragma endregion
		}

	}

}
