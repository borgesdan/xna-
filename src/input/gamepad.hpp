#ifndef XNA_INPUT_GAMEPAD_HPP
#define XNA_INPUT_GAMEPAD_HPP

#include "enumerations.hpp"
#include "../mathhelper.hpp"
#include "../basic-structs.hpp"

namespace xna {
	struct GamePadTriggers {
		double Left;
		double Right;

		constexpr GamePadTriggers() = default;

		constexpr GamePadTriggers(double leftTriggers, double rightTriggers) :
			Left(Math::Clamp(leftTriggers, 0.0, 1.0)),
			Right(Math::Clamp(rightTriggers, 0.0, 1.0)) {
		}

		constexpr bool Equals(GamePadTriggers const& other) const {
			return (Left == other.Left) && (Right == other.Right);
		}

		friend constexpr bool operator==(GamePadTriggers const& left, GamePadTriggers const& right) {
			return left.Equals(right);
		}

		friend constexpr bool operator!=(GamePadTriggers const& left, GamePadTriggers const& right) {
			return !left.Equals(right);
		}
	};

	struct GamePadThumbSticks {
		constexpr GamePadThumbSticks() = default;

		GamePadThumbSticks(Vec2 const& leftPosition, Vec2 const& rightPosition,
			GamePadDeadZone leftDeadZoneMode = GamePadDeadZone::None, GamePadDeadZone rightDeadZoneMode = GamePadDeadZone::None) {

			_left = ApplyDeadZone(leftDeadZoneMode, leftThumbDeadZone, leftPosition);
			_right = ApplyDeadZone(rightDeadZoneMode, rightThumbDeadZone, rightPosition);			

			if (leftPosition.X < -leftThumbDeadZone)
				_virtualButtons = static_cast<Buttons>(toint(_virtualButtons) | toint(Buttons::LeftThumbstickLeft));
			else if (leftPosition.X > leftThumbDeadZone)
				_virtualButtons = static_cast<Buttons>(toint(_virtualButtons) | toint(Buttons::LeftThumbstickRight));

			if (leftPosition.Y < -leftThumbDeadZone)
				_virtualButtons = static_cast<Buttons>(toint(_virtualButtons) | toint(Buttons::LeftThumbstickDown));
			else if (leftPosition.Y > leftThumbDeadZone)
				_virtualButtons = static_cast<Buttons>(toint(_virtualButtons) | toint(Buttons::LeftThumbstickUp));

			if (rightPosition.X < -rightThumbDeadZone)
				_virtualButtons = static_cast<Buttons>(toint(_virtualButtons) | toint(Buttons::RightThumbstickLeft));
			else if (rightPosition.X > rightThumbDeadZone)
				_virtualButtons = static_cast<Buttons>(toint(_virtualButtons) | toint(Buttons::RightThumbstickRight));

			if (rightPosition.Y < -rightThumbDeadZone)
				_virtualButtons = static_cast<Buttons>(toint(_virtualButtons) | toint(Buttons::RightThumbstickDown));
			else if (rightPosition.Y > rightThumbDeadZone)
				_virtualButtons = static_cast<Buttons>(toint(_virtualButtons) | toint(Buttons::RightThumbstickUp));
		}

		constexpr Vec2 Left() const {
			return _left;
		}

		constexpr Vec2 Right() const {
			return _right;
		}

		Buttons _virtualButtons{ 0 };

		constexpr bool Equals(GamePadThumbSticks const& other) const {
			return _left == other._left
				&& _right == other._right;
		}

		friend constexpr bool operator==(GamePadThumbSticks const& left, GamePadThumbSticks const& right) {
			return left.Equals(right);
		}

		friend constexpr bool operator!=(GamePadThumbSticks const& left, GamePadThumbSticks const& right) {
			return !left.Equals(right);
		}

	private:
		static constexpr double leftThumbDeadZone{ 0 };
		static constexpr double rightThumbDeadZone{ 0 };
		Vec2 _left;
		Vec2 _right;

		Vec2 ApplyDeadZone(GamePadDeadZone deadZoneMode, double deadZone, Vec2 thumbstickPosition) {			

			switch (deadZoneMode)
			{
			case GamePadDeadZone::None:
				break;
			case GamePadDeadZone::IndependentAxes:
				thumbstickPosition = ExcludeIndependentAxesDeadZone(thumbstickPosition, deadZone);
				break;
			case GamePadDeadZone::Circular:
				thumbstickPosition = ExcludeCircularDeadZone(thumbstickPosition, deadZone);
				break;
			}

			if (deadZoneMode == GamePadDeadZone::Circular) {
				if (thumbstickPosition.LengthSquared() > 1.0)
					thumbstickPosition.Normalize();
			}
			else {
				thumbstickPosition = Vector2(Math::Clamp(thumbstickPosition.X, -1.0, 1.0), Math::Clamp(thumbstickPosition.Y, -1.0, 1.0));
			}

			return thumbstickPosition;
		}

		Vec2 ExcludeIndependentAxesDeadZone(Vec2 const& value, double deadZone) const {
			return Vec2(ExcludeAxisDeadZone(value.X, deadZone), ExcludeAxisDeadZone(value.Y, deadZone));
		}

		double ExcludeAxisDeadZone(double value, double deadZone) const {
			if (value < -deadZone)
				value += deadZone;
			else if (value > deadZone)
				value -= deadZone;
			else
				return 0.0;

			return value / (1.0 - deadZone);
		}

		Vec2 ExcludeCircularDeadZone(Vec2 const& value, float deadZone) const {
			const auto originalLength = value.Length();

			if (originalLength <= deadZone)
				return Vec2::Zero();

			const auto newLength = (originalLength - deadZone) / (1.0 - deadZone);

			return value * (newLength / originalLength);
		}
	};
}

#endif