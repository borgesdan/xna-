#ifndef XNA_INPUT_JOYSTICK_HPP
#define XNA_INPUT_JOYSTICK_HPP

#include <vector>
#include <string>
#include "enumerations.hpp"
#include "../csharp/integralnumeric.hpp"

namespace xna {
	struct JoystickHat {
		ButtonState Down;
		ButtonState Left;
		ButtonState Right;
		ButtonState Up;

		constexpr bool Equals(JoystickHat const& other) const {
			return (Down == other.Down) &&
				(Left == other.Left) &&
				(Right == other.Right) &&
				(Up == other.Up);
		}

		friend constexpr bool operator==(JoystickHat const& left, JoystickHat const& right) {
			return left.Equals(right);
		}

		friend constexpr bool operator!=(JoystickHat const& left, JoystickHat const& right) {
			return !left.Equals(right);
		}
	};

	struct JoystickState {
		bool IsConnected{ false };
		std::vector<csint> Axes;
		std::vector<ButtonState> Buttons;
		std::vector<JoystickHat> Hats;

		constexpr JoystickState() = default;

		constexpr bool Equals(JoystickState const& other) const {
			return IsConnected == other.IsConnected
				&& Axes == other.Axes
				&& Buttons == other.Buttons
				&& Hats == other.Hats;
		}

		friend constexpr bool operator==(JoystickState const& left, JoystickState const& right) {
			return left.Equals(right);
		}

		friend constexpr bool operator!=(JoystickState const& left, JoystickState const& right) {
			return !left.Equals(right);
		}
	};

	struct JoystickCapabilities {
		bool IsConnected{ false };
		bool IsGamepad{ false };
		std::string Identifier;
		std::string DisplayName;
		csint AxisCount{ 0 };
		csint ButtonCount{ 0 };
		csint HatCount{ 0 };

		constexpr bool Equals(JoystickCapabilities const& other) const {
			return IsConnected == other.IsConnected &&
				Identifier == other.Identifier &&
				IsGamepad == other.IsGamepad &&
				AxisCount == other.AxisCount &&
				ButtonCount == other.ButtonCount &&
				HatCount == other.HatCount;
		}

		friend constexpr bool operator==(JoystickCapabilities const& left, JoystickCapabilities const& right) {
			return left.Equals(right);
		}

		friend constexpr bool operator!=(JoystickCapabilities const& left, JoystickCapabilities const& right) {
			return !left.Equals(right);
		}
	};	
}

#endif