#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <array>
namespace CommonUtilities
{
	class InputHandler
	{
	public:
		InputHandler();

		bool UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam);

		bool IsKeyDown(const int aKeyCode) const;

		bool IsKeyPressed(const int aKeyCode) const;

		bool IsKeyReleased(const int aKeyCode) const;

		bool UpdateMouseInput(UINT aMessage, WPARAM wParam, LPARAM lParam);

		void UpdateInput();

		const POINT GetMouseDeltaPos() const;

		const POINT GetCurrentMousePos() const;

		const int GetScrollWheelDelta() const;

		void SetCursorPosition(const int aX, const int aY);

		void LateUpdate();

		void LockCursorToScreen(bool aIsActivated);

	private:
		bool myCursorIsLocked;
		std::array<bool, 256> myCurrentState;
		std::array<bool, 256> myPreviousState;
		std::array<bool, 256> myInputState;
		int myScrollWheelDelta;
		POINT myMouseDeltaPos;
		POINT myCurrentMousePos;
		POINT myPreviousMousePos;
	};
}