#include "pch.h"
#include "InputHandler.h"

namespace CommonUtilities
{
	InputHandler::InputHandler()
	{
		myInputState.fill(false);
		myCurrentState.fill(false);
		myPreviousState.fill(false);
		myCursorIsLocked = false;
		myCurrentMousePos.x = 0;
		myPreviousMousePos.x = 0;
		myMouseDeltaPos.x = 0;
		myCurrentMousePos.y = 0;
		myPreviousMousePos.y = 0;
		myMouseDeltaPos.y = 0;
		myScrollWheelDelta = 0;
	}

	bool InputHandler::UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (myCursorIsLocked == true)
		{
			HWND windowHandler = GetActiveWindow();
			RECT rectangle;

			if (GetClientRect(windowHandler, &rectangle))
			{
				MapWindowPoints(windowHandler, NULL, reinterpret_cast<POINT*>(&rectangle), 2);
				ClipCursor(&rectangle);
			}
		}

		if (UpdateMouseInput(message, wParam, lParam))
		{
			return true;
		}

		switch (message)
		{
		case WM_KEYDOWN:
			myInputState[wParam] = true;
			return true;
			break;
		case WM_KEYUP:
			myInputState[wParam] = false;
			return true;
			break;
		default:
			return false;
			break;
		}
	}

	bool InputHandler::IsKeyDown(const int aKeyCode) const
	{
		if (myCurrentState[aKeyCode] == true && myPreviousState[aKeyCode] == false)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool InputHandler::IsKeyPressed(const int aKeyCode) const
	{
		if (myCurrentState[aKeyCode] == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool InputHandler::IsKeyReleased(const int aKeyCode) const
	{
		if (myPreviousState[aKeyCode] == true && myCurrentState[aKeyCode] == false)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool InputHandler::UpdateMouseInput(UINT aMessage, WPARAM wParam, LPARAM lParam)
	{
		switch (aMessage)
		{
		case WM_MOUSEMOVE:
			myCurrentMousePos.x = GET_X_LPARAM(lParam);
			myCurrentMousePos.y = GET_Y_LPARAM(lParam);
			if (myPreviousMousePos.x != myCurrentMousePos.x || myPreviousMousePos.y != myCurrentMousePos.y)
			{
				myMouseDeltaPos.x = myCurrentMousePos.x - myPreviousMousePos.x;
				myMouseDeltaPos.y = myCurrentMousePos.y - myPreviousMousePos.y;
				myPreviousMousePos = myCurrentMousePos;
				return true;
			}
			else
			{
				return false;
			}
			break;
		case WM_LBUTTONDOWN:
			myInputState[VK_LBUTTON] = true;
			return true;
			break;
		case WM_LBUTTONUP:
			myInputState[VK_LBUTTON] = false;
			return true;
			break;
		case WM_RBUTTONDOWN:
			myInputState[VK_RBUTTON] = true;
			return true;
			break;
		case WM_RBUTTONUP:
			myInputState[VK_RBUTTON] = false;
			return true;
			break;
		case WM_MBUTTONDOWN:
			myInputState[VK_MBUTTON] = true;
			return true;
			break;
		case WM_MBUTTONUP:
			myInputState[MK_MBUTTON] = false;
			return true;
			break;
		case WM_MOUSEWHEEL:
			myScrollWheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			return true;
			break;
		default:
			return false;
			break;
		}
	}

	void InputHandler::UpdateInput()
	{
		for (UINT keyIndex = 0; keyIndex < myInputState.size(); keyIndex++)
		{
			myPreviousState[keyIndex] = myCurrentState[keyIndex];
			myCurrentState[keyIndex] = myInputState[keyIndex];
		}
	}

	const POINT InputHandler::GetMouseDeltaPos() const
	{
		return myMouseDeltaPos;
	}

	const POINT InputHandler::GetCurrentMousePos() const
	{
		return myCurrentMousePos;
	}

	const int InputHandler::GetScrollWheelDelta() const
	{
		return myScrollWheelDelta;
	}

	void InputHandler::SetCursorPosition(const int aX, const int aY)
	{
		SetCursorPos(aX, aY);
	}

	void InputHandler::LateUpdate()
	{
		//myCurrentMousePos.x = 0;
		//myPreviousMousePos.x = 0;
		myMouseDeltaPos.x = 0;
		//myCurrentMousePos.y = 0;
		//myPreviousMousePos.y = 0;
		myMouseDeltaPos.y = 0;
		myScrollWheelDelta = 0;
	}
	void InputHandler::LockCursorToScreen(bool aIsActivated)
	{
		myCursorIsLocked = aIsActivated;
	}
}