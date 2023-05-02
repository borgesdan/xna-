#ifndef XNA_INPUT_MOUSE_HPP
#define XNA_INPUT_MOUSE_HPP

#include "../csharp/integralnumeric.hpp"
#include "enumerations.hpp"
#include "../basic-structs.hpp"

namespace xna {
	enum class MouseButton : csbyte {
		LeftButton = 1,
		RightButton = 2,
		MiddleButton = 4,
		XButton1 = 8,
		XButton2 = 16
	};

	struct MouseState {
		constexpr MouseState() {}

		constexpr MouseState(csint x, csint y, csint scrollWheel,
			ButtonState leftButton, ButtonState middleButton, ButtonState rightButton,
			ButtonState xButton1, ButtonState xButton2, csint horizontalScrollWheel = 0) :
			X(x), Y(y),
			ScrollWheelValue(scrollWheel),
			HorizontalScrollWheelValue(horizontalScrollWheel) {
			_buttons = tobyte(
				(leftButton == ButtonState::Pressed ? LeftButtonFlag : 0) |
				(rightButton == ButtonState::Pressed ? RightButtonFlag : 0) |
				(middleButton == ButtonState::Pressed ? MiddleButtonFlag : 0) |
				(xButton1 == ButtonState::Pressed ? XButton1Flag : 0) |
				(xButton2 == ButtonState::Pressed ? XButton2Flag : 0));
		}	

		constexpr bool Equals(MouseState const& other) const {
			return 
				X == other.X &&
				Y == other.Y &&
				_buttons == other._buttons &&
				ScrollWheelValue == other.ScrollWheelValue &&
				HorizontalScrollWheelValue == other.HorizontalScrollWheelValue;
		}

		friend constexpr bool operator==(MouseState const& left, MouseState const& right) {
			return left.Equals(right);
		}

		friend constexpr bool operator!=(MouseState const& left, MouseState const& right) {
			return !left.Equals(right);
		}

	private:
		static constexpr csbyte LeftButtonFlag = static_cast<csbyte>(MouseButton::LeftButton);
		static constexpr csbyte RightButtonFlag = static_cast<csbyte>(MouseButton::RightButton);
		static constexpr csbyte MiddleButtonFlag = static_cast<csbyte>(MouseButton::MiddleButton);
		static constexpr csbyte XButton1Flag = static_cast<csbyte>(MouseButton::XButton1);
		static constexpr csbyte XButton2Flag = static_cast<csbyte>(MouseButton::XButton2);

		csint ScrollWheelValue{ 0 };
		csint HorizontalScrollWheelValue{ 0 };
		csbyte _buttons{ 0 };

	public:
		csint X{ 0 };
		csint Y{ 0 };
		
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
	};
}

#endif