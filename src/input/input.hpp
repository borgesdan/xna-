#ifndef XNA_INPUT_INPUT_HPP
#define XNA_INPUT_INPUT_HPP

#include <memory>
#include "mouse.hpp"
#include "gamepad.hpp"
#include "joystick.hpp"
#include "keyboard.hpp"
#include "../gamewindow.hpp"

namespace xna {
	struct Mouse {
		static MouseState GetState(GameWindow const& window) {
			return PlaftformGetState(window);
		}

		static MouseState GetState() {
			if (PrimaryWindow != nullptr)
				return GetState(*PrimaryWindow);

			return _defaultState;
		}

		static void SetPosition(int x, int y) {
			PlatformSetPosition(x, y);
		}

		static void SetCursor(MouseCursor const& cursor) {
			PlatformSetCursor(cursor);
		}

	private:
		static constexpr MouseState _defaultState = MouseState();
		static std::shared_ptr<GameWindow> PrimaryWindow;

		static MouseState PlaftformGetState(GameWindow const& window) {
			return window.MouseState;
		}

		static void PlatformSetPosition(int x, int y) {
			PrimaryWindow->MouseState.X = x;
			PrimaryWindow->MouseState.Y = y;

			//var pt = _window.PointToScreen(new System.Drawing.Point(x, y));
			//SetCursorPos(pt.X, pt.Y);
		}

		static void PlatformSetCursor(MouseCursor const& cursor) {
		}
	};

	struct GamePad {
		static GamePadCapabilities GetCapabilities(PlayerIndex playerIndex) {
			const auto index = static_cast<int>(playerIndex);

			if (index < 0 || index >= PlatformGetMaxNumberOfGamePads())
				return GamePadCapabilities();

			return PlatformGetCapabilities(index);
		}

		static GamePadState GetState(PlayerIndex playerIndex, GamePadDeadZone leftDeadZoneMode = GamePadDeadZone::IndependentAxes,
			GamePadDeadZone rightDeadZoneMode = GamePadDeadZone::IndependentAxes) {
			const auto index = static_cast<int>(playerIndex);

			if (index < 0 || index >= PlatformGetMaxNumberOfGamePads())
				return GamePadState::Default();

			return PlatformGetState(index, leftDeadZoneMode, rightDeadZoneMode);
		}

		static constexpr bool SetVibration(PlayerIndex playerIndex, double leftMotor, double rightMotor, double leftTrigger = 0.0, double rightTrigger = 0.0) {
			const auto index = static_cast<int>(playerIndex);

			if (index < 0 || index >= PlatformGetMaxNumberOfGamePads())
				return false;

			return PlatformSetVibration(index, Math::Clamp(leftMotor, 0.0f, 1.0f), Math::Clamp(rightMotor, 0.0f, 1.0f), Math::Clamp(leftTrigger, 0.0f, 1.0f), Math::Clamp(rightTrigger, 0.0f, 1.0f));
		}

		static constexpr int MaximumGamePadCount() {
			return PlatformGetMaxNumberOfGamePads();
		}

	private:
		static constexpr int PlatformGetMaxNumberOfGamePads() {
			return 4;
		}

		static GamePadCapabilities PlatformGetCapabilities(int index) {
			return GamePadCapabilities();
		}

		static GamePadState PlatformGetState(int index, GamePadDeadZone leftDeadZoneMode, GamePadDeadZone rightDeadZoneMode) {
			return GamePadState();
		}

		static bool PlatformSetVibration(int index, double leftMotor, double rightMotor, double leftTrigger, double rightTrigger) {
			return false;
		}
	};

	struct Joystick {
		static constexpr bool IsSupported() {
			return PlatformIsSupported();
		}

		static constexpr csint LasConnectedIndex() {
			return PlatformLastConnectedIndex();
		}

		static constexpr JoystickCapabilities GetCapabilities(csint index) {
			return PlatformGetCapabilities(index);
		}

		static constexpr JoystickState GetState(csint index) {
			return PlatformGetState(index);
		}

		static constexpr void Getstate(JoystickState& joystickState, csint index) {
			PlatformLastConnectedIndex(joystickState, index);
		}


	private:
		static constexpr bool PlatformIsSupported() {
			return false;
		}

		static constexpr JoystickCapabilities PlatformGetCapabilities(csint index) {
			return JoystickCapabilities();
		}

		static constexpr JoystickState PlatformGetState(csint index) {
			return JoystickState();
		}

		static constexpr csint PlatformLastConnectedIndex() {
			return -1;
		}

		static constexpr void PlatformLastConnectedIndex(JoystickState& joystickState, csint index) {
		}
	};

	struct Keyboard {
		static KeyboardState GetState();

		static bool GetKeyboard(std::vector<csbyte>& lpKeyState);

		static constexpr void SetActive(bool isActive) {
			_isActive = isActive;

			if (!_isActive)
				_keys.clear();
		}

	private:
		static std::vector<csbyte> DefinedKeyCodes;
		static std::vector<csbyte> _keyState;
		static std::vector<Keys> _keys;
		static bool _isActive;

		static KeyboardState PlatformGetState();

		static constexpr bool IsKeyReleased(csbyte keyCode) {
			return ((_keyState[keyCode] & 0x80) == 0);
		}

		static constexpr std::vector<csbyte> GetDefinedKeyCodes() {
			std::vector<csbyte> codes;
			codes.push_back(tobyte(Keys::Back));
			codes.push_back(tobyte(Keys::Tab));
			codes.push_back(tobyte(Keys::Enter));
			codes.push_back(tobyte(Keys::CapsLock));
			codes.push_back(tobyte(Keys::Escape));
			codes.push_back(tobyte(Keys::Space));
			codes.push_back(tobyte(Keys::PageUp));
			codes.push_back(tobyte(Keys::PageDown));
			codes.push_back(tobyte(Keys::End));
			codes.push_back(tobyte(Keys::Home));
			codes.push_back(tobyte(Keys::Left));
			codes.push_back(tobyte(Keys::Up));
			codes.push_back(tobyte(Keys::Right));
			codes.push_back(tobyte(Keys::Down));
			codes.push_back(tobyte(Keys::Select));
			codes.push_back(tobyte(Keys::Print));
			codes.push_back(tobyte(Keys::Execute));
			codes.push_back(tobyte(Keys::PrintScreen));
			codes.push_back(tobyte(Keys::Insert));
			codes.push_back(tobyte(Keys::Delete));
			codes.push_back(tobyte(Keys::Help));
			codes.push_back(tobyte(Keys::D));
			codes.push_back(tobyte(Keys::D));
			codes.push_back(tobyte(Keys::D));
			codes.push_back(tobyte(Keys::D));
			codes.push_back(tobyte(Keys::D));
			codes.push_back(tobyte(Keys::D));
			codes.push_back(tobyte(Keys::D));
			codes.push_back(tobyte(Keys::D));
			codes.push_back(tobyte(Keys::D));
			codes.push_back(tobyte(Keys::D));
			codes.push_back(tobyte(Keys::A));
			codes.push_back(tobyte(Keys::B));
			codes.push_back(tobyte(Keys::C));
			codes.push_back(tobyte(Keys::D));
			codes.push_back(tobyte(Keys::E));
			codes.push_back(tobyte(Keys::F));
			codes.push_back(tobyte(Keys::G));
			codes.push_back(tobyte(Keys::H));
			codes.push_back(tobyte(Keys::I));
			codes.push_back(tobyte(Keys::J));
			codes.push_back(tobyte(Keys::K));
			codes.push_back(tobyte(Keys::L));
			codes.push_back(tobyte(Keys::M));
			codes.push_back(tobyte(Keys::N));
			codes.push_back(tobyte(Keys::O));
			codes.push_back(tobyte(Keys::P));
			codes.push_back(tobyte(Keys::Q));
			codes.push_back(tobyte(Keys::R));
			codes.push_back(tobyte(Keys::S));
			codes.push_back(tobyte(Keys::T));
			codes.push_back(tobyte(Keys::U));
			codes.push_back(tobyte(Keys::V));
			codes.push_back(tobyte(Keys::W));
			codes.push_back(tobyte(Keys::X));
			codes.push_back(tobyte(Keys::Y));
			codes.push_back(tobyte(Keys::Z));
			codes.push_back(tobyte(Keys::LeftWindows));
			codes.push_back(tobyte(Keys::RightWindows));
			codes.push_back(tobyte(Keys::Apps));
			codes.push_back(tobyte(Keys::Sleep));
			codes.push_back(tobyte(Keys::NumPad0));
			codes.push_back(tobyte(Keys::NumPad1));
			codes.push_back(tobyte(Keys::NumPad2));
			codes.push_back(tobyte(Keys::NumPad3));
			codes.push_back(tobyte(Keys::NumPad4));
			codes.push_back(tobyte(Keys::NumPad5));
			codes.push_back(tobyte(Keys::NumPad6));
			codes.push_back(tobyte(Keys::NumPad7));
			codes.push_back(tobyte(Keys::NumPad8));
			codes.push_back(tobyte(Keys::NumPad9));
			codes.push_back(tobyte(Keys::Multiply));
			codes.push_back(tobyte(Keys::Add));
			codes.push_back(tobyte(Keys::Separator));
			codes.push_back(tobyte(Keys::Subtract));
			codes.push_back(tobyte(Keys::Decimal));
			codes.push_back(tobyte(Keys::Divide));
			codes.push_back(tobyte(Keys::F1));
			codes.push_back(tobyte(Keys::F2));
			codes.push_back(tobyte(Keys::F3));
			codes.push_back(tobyte(Keys::F4));
			codes.push_back(tobyte(Keys::F5));
			codes.push_back(tobyte(Keys::F6));
			codes.push_back(tobyte(Keys::F7));
			codes.push_back(tobyte(Keys::F8));
			codes.push_back(tobyte(Keys::F9));
			codes.push_back(tobyte(Keys::F10));
			codes.push_back(tobyte(Keys::F11));
			codes.push_back(tobyte(Keys::F12));
			codes.push_back(tobyte(Keys::F13));
			codes.push_back(tobyte(Keys::F14));
			codes.push_back(tobyte(Keys::F15));
			codes.push_back(tobyte(Keys::F16));
			codes.push_back(tobyte(Keys::F17));
			codes.push_back(tobyte(Keys::F18));
			codes.push_back(tobyte(Keys::F19));
			codes.push_back(tobyte(Keys::F20));
			codes.push_back(tobyte(Keys::F21));
			codes.push_back(tobyte(Keys::F22));
			codes.push_back(tobyte(Keys::F23));
			codes.push_back(tobyte(Keys::F24));
			codes.push_back(tobyte(Keys::NumLock));
			codes.push_back(tobyte(Keys::Scroll));
			codes.push_back(tobyte(Keys::LeftShift));
			codes.push_back(tobyte(Keys::RightShift));
			codes.push_back(tobyte(Keys::LeftControl));
			codes.push_back(tobyte(Keys::RightControl));
			codes.push_back(tobyte(Keys::LeftAlt));
			codes.push_back(tobyte(Keys::RightAlt));
			codes.push_back(tobyte(Keys::BrowserBack));
			codes.push_back(tobyte(Keys::BrowserForward));
			codes.push_back(tobyte(Keys::BrowserRefresh));
			codes.push_back(tobyte(Keys::BrowserStop));
			codes.push_back(tobyte(Keys::BrowserSearch));
			codes.push_back(tobyte(Keys::BrowserFavorites));
			codes.push_back(tobyte(Keys::BrowserHome));
			codes.push_back(tobyte(Keys::VolumeMute));
			codes.push_back(tobyte(Keys::VolumeDown));
			codes.push_back(tobyte(Keys::VolumeUp));
			codes.push_back(tobyte(Keys::MediaNextTrack));
			codes.push_back(tobyte(Keys::MediaPreviousTrack));
			codes.push_back(tobyte(Keys::MediaStop));
			codes.push_back(tobyte(Keys::MediaPlayPause));
			codes.push_back(tobyte(Keys::LaunchMail));
			codes.push_back(tobyte(Keys::SelectMedia));
			codes.push_back(tobyte(Keys::LaunchApplication1));
			codes.push_back(tobyte(Keys::LaunchApplication2));
			codes.push_back(tobyte(Keys::OemSemicolon));
			codes.push_back(tobyte(Keys::OemPlus));
			codes.push_back(tobyte(Keys::OemComma));
			codes.push_back(tobyte(Keys::OemMinus));
			codes.push_back(tobyte(Keys::OemPeriod));
			codes.push_back(tobyte(Keys::OemQuestion));
			codes.push_back(tobyte(Keys::OemTilde));
			codes.push_back(tobyte(Keys::OemOpenBrackets));
			codes.push_back(tobyte(Keys::OemPipe));
			codes.push_back(tobyte(Keys::OemCloseBrackets));
			codes.push_back(tobyte(Keys::OemQuotes));
			codes.push_back(tobyte(Keys::Oem8));
			codes.push_back(tobyte(Keys::OemBackslash));
			codes.push_back(tobyte(Keys::ProcessKey));
			codes.push_back(tobyte(Keys::Attn));
			codes.push_back(tobyte(Keys::Crsel));
			codes.push_back(tobyte(Keys::Exsel));
			codes.push_back(tobyte(Keys::EraseEof));
			codes.push_back(tobyte(Keys::Play));
			codes.push_back(tobyte(Keys::Zoom));
			codes.push_back(tobyte(Keys::Pa1));
			codes.push_back(tobyte(Keys::OemClear));
			codes.push_back(tobyte(Keys::ChatPadGreen));
			codes.push_back(tobyte(Keys::ChatPadOrange));
			codes.push_back(tobyte(Keys::Pause));
			codes.push_back(tobyte(Keys::ImeConvert));
			codes.push_back(tobyte(Keys::ImeNoConvert));
			codes.push_back(tobyte(Keys::Kana));
			codes.push_back(tobyte(Keys::Kanji));
			codes.push_back(tobyte(Keys::OemAuto));
			codes.push_back(tobyte(Keys::OemCopy));
			codes.push_back(tobyte(Keys::OemEnlW));

			return codes;
		}
	};
}

#endif