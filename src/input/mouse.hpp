#ifndef XNA_INPUT_MOUSE_HPP
#define XNA_INPUT_MOUSE_HPP

#include "../csharp/integralnumeric.hpp"
#include "../basic-structs.hpp"
#include "enumerations.hpp"
#include <Windows.h>

namespace xna {
	class Texture2D;

	struct MouseState {
		constexpr MouseState() = default;

		constexpr MouseState(csint x, csint y, csint scrollWheel, ButtonState leftButton, ButtonState middleButton, ButtonState rightButton,
			ButtonState xButton1, ButtonState xButton2, csint horizontalScrollWheel = 0) :
			X(x), Y(y),	ScrollWheelValue(scrollWheel), HorizontalScrollWheelValue(horizontalScrollWheel) {
			_buttons = tobyte(
				(leftButton == ButtonState::Pressed ? LeftButtonFlag : 0) |
				(rightButton == ButtonState::Pressed ? RightButtonFlag : 0) |
				(middleButton == ButtonState::Pressed ? MiddleButtonFlag : 0) |
				(xButton1 == ButtonState::Pressed ? XButton1Flag : 0) |
				(xButton2 == ButtonState::Pressed ? XButton2Flag : 0));
		}	

		constexpr bool Equals(MouseState const& other) const {
			return X == other.X 
				&& Y == other.Y 
				&& _buttons == other._buttons 
				&& ScrollWheelValue == other.ScrollWheelValue 
				&& HorizontalScrollWheelValue == other.HorizontalScrollWheelValue;
		}

		friend constexpr bool operator==(MouseState const& left, MouseState const& right) {
			return left.Equals(right);
		}

		friend constexpr bool operator!=(MouseState const& left, MouseState const& right) {
			return !left.Equals(right);
		}	
		
		constexpr Point Position() const {
			return { X, Y };
		}

		constexpr ButtonState LeftButton() const {
			return ((_buttons & LeftButtonFlag) > 0) ? ButtonState::Pressed : ButtonState::Released;
		}	

		constexpr void LeftButton(ButtonState value) {
			if (value == ButtonState::Pressed)
				_buttons = tobyte(_buttons | LeftButtonFlag);
			else
				_buttons = tobyte(_buttons & (~LeftButtonFlag));
		}

		constexpr ButtonState MiddleButton() const {
			return ((_buttons & MiddleButtonFlag) > 0) ? ButtonState::Pressed : ButtonState::Released;
		}

		constexpr void MiddleButton(ButtonState value) {
			if (value == ButtonState::Pressed)
				_buttons = tobyte(_buttons | MiddleButtonFlag);
			else
				_buttons = tobyte(_buttons & (~MiddleButtonFlag));
		}

		constexpr ButtonState RightButton() const {
			return ((_buttons & RightButtonFlag) > 0) ? ButtonState::Pressed : ButtonState::Released;
		}

		constexpr void RightButton(ButtonState value) {
			if (value == ButtonState::Pressed)
				_buttons = tobyte(_buttons | RightButtonFlag);
			else
				_buttons = tobyte(_buttons & (~RightButtonFlag));
		}

		constexpr ButtonState XButton1() const {
			return ((_buttons & XButton1Flag) > 0) ? ButtonState::Pressed : ButtonState::Released;
		}

		constexpr void XButton1(ButtonState value) {
			if (value == ButtonState::Pressed)
				_buttons = tobyte(_buttons | XButton1Flag);
			else
				_buttons = tobyte(_buttons & (~XButton1Flag));
		}

		constexpr ButtonState XButton2() const {
			return ((_buttons & XButton2Flag) > 0) ? ButtonState::Pressed : ButtonState::Released;
		}

		constexpr void XButton2(ButtonState value) {
			if (value == ButtonState::Pressed)
				_buttons = tobyte(_buttons | XButton2Flag);
			else
				_buttons = tobyte(_buttons & (~XButton2Flag));
		}

	public:
		csint X{ 0 };
		csint Y{ 0 };

	private:
		static constexpr csbyte LeftButtonFlag = static_cast<csbyte>(MouseButton::Left);
		static constexpr csbyte RightButtonFlag = static_cast<csbyte>(MouseButton::Right);
		static constexpr csbyte MiddleButtonFlag = static_cast<csbyte>(MouseButton::Middle);
		static constexpr csbyte XButton1Flag = static_cast<csbyte>(MouseButton::X1);
		static constexpr csbyte XButton2Flag = static_cast<csbyte>(MouseButton::X2);

		csint ScrollWheelValue{ 0 };
		csint HorizontalScrollWheelValue{ 0 };
		csbyte _buttons{ 0 };
	};

	struct MouseCursor {	
		
		HCURSOR _cursor;

		MouseCursor(HCURSOR cursor);			

		static MouseCursor Arrow;
		static MouseCursor IBeam;
		static MouseCursor Wait;
		static MouseCursor Crosshair;
		static MouseCursor WaitArrow;
		static MouseCursor SizeNWSE;
		static MouseCursor SizeNESW;
		static MouseCursor SizeWE;
		static MouseCursor SizeNS;
		static MouseCursor SizeAll;
		static MouseCursor No;
		static MouseCursor Hand;

		static MouseCursor PlatformFromTexture2D(Texture2D texture, int originx, int originy);
		static MouseCursor FromTexture2D(Texture2D texture, int originx, int originy);
	};

	struct Mouse {

	};
}

#endif