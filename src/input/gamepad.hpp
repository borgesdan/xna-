#ifndef XNA_INPUT_GAMEPAD_HPP
#define XNA_INPUT_GAMEPAD_HPP

#include <string>
#include <vector>
#include "enumerations.hpp"
#include "../mathhelper.hpp"
#include "../basic-structs.hpp"
#include "../enumerations.hpp"

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

		constexpr GamePadThumbSticks(Vec2 const& leftPosition, Vec2 const& rightPosition,
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
		static constexpr double leftThumbDeadZone{ 0 }; //TODO
		static constexpr double rightThumbDeadZone{ 0 }; //TODO
		Vec2 _left;
		Vec2 _right;

		constexpr Vec2 ApplyDeadZone(GamePadDeadZone deadZoneMode, double deadZone, Vec2 thumbstickPosition) {

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

		constexpr Vec2 ExcludeIndependentAxesDeadZone(Vec2 const& value, double deadZone) const {
			return Vec2(ExcludeAxisDeadZone(value.X, deadZone), ExcludeAxisDeadZone(value.Y, deadZone));
		}

		constexpr double ExcludeAxisDeadZone(double value, double deadZone) const {
			if (value < -deadZone)
				value += deadZone;
			else if (value > deadZone)
				value -= deadZone;
			else
				return 0.0;

			return value / (1.0 - deadZone);
		}

		constexpr Vec2 ExcludeCircularDeadZone(Vec2 const& value, float deadZone) const {
			const auto originalLength = value.Length();

			if (originalLength <= deadZone)
				return Vec2::Zero();

			const auto newLength = (originalLength - deadZone) / (1.0 - deadZone);

			return value * (newLength / originalLength);
		}
	};

	struct GamePadCapabilities {
		bool IsConnected{ false };
		bool HasAButton{ false };
		bool HasBackButton{ false };
		bool HasBButton{ false };
		bool HasDPadDownButton{ false };
		bool HasDPadLeftButton{ false };
		bool HasDPadRightButton{ false };
		bool HasDPadUpButton{ false };
		bool HasLeftShoulderButton{ false };
		bool HasLeftStickButton{ false };
		bool HasRightShoulderButton{ false };
		bool HasRightStickButton{ false };
		bool HasStartButton{ false };
		bool HasXButton{ false };
		bool HasYButton{ false };
		bool HasBigButton{ false };
		bool HasLeftXThumbStick{ false };
		bool HasLeftYThumbStick{ false };
		bool HasRightXThumbStick{ false };
		bool HasRightYThumbStick{ false };
		bool HasLeftTrigger{ false };
		bool HasRightTrigger{ false };
		bool HasLeftVibrationMotor{ false };
		bool HasRightVibrationMotor{ false };
		bool HasVoiceSupport{ false };
		std::string DisplayName;
		std::string Identifier;
		GamePadType GamePadType;

		constexpr bool Equals(GamePadCapabilities const& other) const {
			return
				(DisplayName == other.DisplayName);
			(Identifier == other.Identifier);
			(IsConnected == other.IsConnected);
			(HasAButton == other.HasAButton);
			(HasBackButton == other.HasBackButton);
			(HasBButton == other.HasBButton);
			(HasDPadDownButton == other.HasDPadDownButton);
			(HasDPadLeftButton == other.HasDPadLeftButton);
			(HasDPadRightButton == other.HasDPadRightButton);
			(HasDPadUpButton == other.HasDPadUpButton);
			(HasLeftShoulderButton == other.HasLeftShoulderButton);
			(HasLeftStickButton == other.HasLeftStickButton);
			(HasRightShoulderButton == other.HasRightShoulderButton);
			(HasRightStickButton == other.HasRightStickButton);
			(HasStartButton == other.HasStartButton);
			(HasXButton == other.HasXButton);
			(HasYButton == other.HasYButton);
			(HasBigButton == other.HasBigButton);
			(HasLeftXThumbStick == other.HasLeftXThumbStick);
			(HasLeftYThumbStick == other.HasLeftYThumbStick);
			(HasRightXThumbStick == other.HasRightXThumbStick);
			(HasRightYThumbStick == other.HasRightYThumbStick);
			(HasLeftTrigger == other.HasLeftTrigger);
			(HasRightTrigger == other.HasRightTrigger);
			(HasLeftVibrationMotor == other.HasLeftVibrationMotor);
			(HasRightVibrationMotor == other.HasRightVibrationMotor);
			(HasVoiceSupport == other.HasVoiceSupport);
			(GamePadType == other.GamePadType);
		}

		friend constexpr bool operator==(GamePadCapabilities const& right, GamePadCapabilities const& left) {
			return left.Equals(right);
		}

		friend constexpr bool operator!=(GamePadCapabilities const& right, GamePadCapabilities const& left) {
			return !left.Equals(right);
		}
	};

	struct GamePadButtons {
		Buttons _buttons;

		constexpr GamePadButtons() = default;

		constexpr GamePadButtons(Buttons buttons) : _buttons(buttons) {
		}

		constexpr GamePadButtons(std::vector<Buttons> buttons) {
			for (size_t i = 0; i < buttons.size(); ++i) {
				const auto& b = buttons[i];
				_buttons = static_cast<Buttons>(toint(_buttons) | toint(b));
			}
		}

		constexpr ButtonState A() {
			return Get(Buttons::A);
		}

		constexpr ButtonState B() {
			return Get(Buttons::B);
		}

		constexpr ButtonState Back() {
			return Get(Buttons::Back);
		}

		constexpr ButtonState X() {
			return Get(Buttons::X);
		}

		constexpr ButtonState Y() {
			return Get(Buttons::Y);
		}

		constexpr ButtonState Start() {
			return Get(Buttons::Start);
		}

		constexpr ButtonState LeftShoulder() {
			return Get(Buttons::LeftShoulder);
		}

		constexpr ButtonState LeftStick() {
			return Get(Buttons::LeftStick);
		}

		constexpr ButtonState RightShoulder() {
			return Get(Buttons::RightShoulder);
		}

		constexpr ButtonState RightStick() {
			return Get(Buttons::RightStick);
		}

		constexpr ButtonState BigButton() {
			return Get(Buttons::BigButton);
		}

		constexpr bool Equals(GamePadButtons const& other) const {
			return _buttons == other._buttons;
		}

		friend constexpr bool operator==(GamePadButtons const& right, GamePadButtons const& left) {
			return left.Equals(right);
		}

		friend constexpr bool operator!=(GamePadButtons const& right, GamePadButtons const& left) {
			return !left.Equals(right);
		}

	private:
		constexpr ButtonState Get(Buttons button) {
			return ((static_cast<Buttons>(toint(_buttons) & toint(button))) == button)
				? ButtonState::Pressed
				: ButtonState::Released;
		}
	};

	struct GamePadDPad {
		ButtonState Down{ ButtonState::Released };
		ButtonState Left{ ButtonState::Released };
		ButtonState Right{ ButtonState::Released };
		ButtonState Up{ ButtonState::Released };

		constexpr GamePadDPad() = default;

		constexpr GamePadDPad(ButtonState up, ButtonState down, ButtonState left, ButtonState right) :
			Up(up), Down(down), Left(left), Right(right) {
		}

		constexpr GamePadDPad(std::vector<Buttons> const& buttons) {
			for (size_t i = 0; i < buttons.size(); ++i) {
				ConvertButtonToDirection(buttons[i]);
			}
		}

		constexpr GamePadDPad(Buttons button) {
			ConvertButtonToDirection(button);
		}

		constexpr bool Equals(GamePadDPad const& other) const {
			return (Down == other.Down)
				&& (Left == other.Left)
				&& (Right == other.Right)
				&& (Up == other.Up);
		}

		friend constexpr bool operator==(GamePadDPad const& left, GamePadDPad const& right) {
			return left.Equals(right);
		}

		friend constexpr bool operator!=(GamePadDPad const& left, GamePadDPad const& right) {
			return !left.Equals(right);
		}

	private:
		void constexpr ConvertButtonToDirection(Buttons button) {
			SetPressedState(button, Buttons::DPadDown, Down);
			SetPressedState(button, Buttons::DPadLeft, Left);
			SetPressedState(button, Buttons::DPadRight, Right);
			SetPressedState(button, Buttons::DPadUp, Up);
		}

		static constexpr void SetPressedState(Buttons current, Buttons value, ButtonState& state) {
			if (static_cast<Buttons>(toint(current) & toint(value)) == value)
				state = ButtonState::Pressed;
		}
	};

	struct GamePadState {

		constexpr GamePadState() = default;

		GamePadState(GamePadThumbSticks thumbSticks, GamePadTriggers triggers, GamePadButtons buttons, GamePadDPad dPad) :
			ThumbSticks(thumbSticks), Triggers(triggers), Buttons(buttons), DPad(dPad), IsConnected(true) {
			PlatformConstruct();
		}

		GamePadState(Vector2 const& leftThumbStick, Vector2 const& rightThumbStick, double leftTrigger, double rightTrigger, Buttons button) :
			GamePadState(GamePadThumbSticks(leftThumbStick, rightThumbStick), GamePadTriggers(leftTrigger, rightTrigger), GamePadButtons(button), GamePadDPad(button)) {

		}

		GamePadState(Vector2 const& leftThumbStick, Vector2 const& rightThumbStick, double leftTrigger, double rightTrigger, std::vector<Buttons> buttons) :
			GamePadState(GamePadThumbSticks(leftThumbStick, rightThumbStick), GamePadTriggers(leftTrigger, rightTrigger), GamePadButtons(buttons), GamePadDPad(buttons)) {

		}

		void PlatformConstruct() {
		}

		static constexpr GamePadState Default() {
			return GamePadState();
		}

		constexpr bool IsButtonDown(Buttons button) const {
			if (button == xna::Buttons::None) 
				return false;

			const auto btns = GetVirtualButtons();
			const auto result = static_cast<xna::Buttons>(toint(btns) & toint(button));

			return result == button;
		}

		constexpr bool IsButtonUp(Buttons button) const {
			const auto btns = GetVirtualButtons();
			const auto result = static_cast<xna::Buttons>(toint(btns) & toint(button));

			return result != button;
		}

		constexpr bool Equals(GamePadState const& other) const {
			return (IsConnected == other.IsConnected) 
				&& (PacketNumber == other.PacketNumber) 
				&& (Buttons == other.Buttons) 
				&& (DPad == other.DPad) 
				&& (ThumbSticks == other.ThumbSticks) 
				&& (Triggers == other.Triggers);
		}

		friend constexpr bool operator==(GamePadState const& left, GamePadState const& right) {
			return left.Equals(right);
		}

		friend constexpr bool operator!=(GamePadState const& left, GamePadState const& right) {
			return !left.Equals(right);
		}

	public:
		bool IsConnected{ false };
		csint PacketNumber{ 0 };
		GamePadButtons Buttons;
		GamePadDPad DPad;
		GamePadThumbSticks ThumbSticks;
		GamePadTriggers Triggers;

	private:
		constexpr xna::Buttons GetVirtualButtons() const {
			auto result = toint(Buttons._buttons) | toint(ThumbSticks._virtualButtons);

			if (DPad.Down == ButtonState::Pressed)
				result |= toint(Buttons::DPadDown);
			if (DPad.Up == ButtonState::Pressed)
				result |= toint(Buttons::DPadUp);
			if (DPad.Left == ButtonState::Pressed)
				result |= toint(Buttons::DPadLeft);
			if (DPad.Right == ButtonState::Pressed)
				result |= toint(Buttons::DPadRight);

			return static_cast<xna::Buttons>(result);
		}
	};	
}

#endif