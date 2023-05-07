#include "input.hpp"
#include <Windows.h>

//Keyboard
namespace xna {
	std::vector<csbyte> Keyboard::DefinedKeyCodes = GetDefinedKeyCodes();
	std::vector<csbyte> Keyboard::_keyState = std::vector<csbyte>(256);
	std::vector<Keys> Keyboard::_keys = std::vector<Keys>(10);
	bool Keyboard::_isActive = false;

	KeyboardState Keyboard::GetState() {
		return PlatformGetState();
	}

	bool Keyboard::GetKeyboard(std::vector<csbyte>& lpKeyState) {
		return GetKeyboardState(lpKeyState.data());
	}

	KeyboardState Keyboard::PlatformGetState() {
		if (_isActive && GetKeyboard(_keyState)) {
			_keys.erase(
				std::remove_if(_keys.begin(), _keys.end(), [](Keys const& k) {
					return IsKeyReleased(tobyte(k));
					}),
				_keys.end());

			for (size_t i = 0; i < DefinedKeyCodes.size(); ++i) {
				const auto& keyCode = DefinedKeyCodes[i];

				if (IsKeyReleased(keyCode))
					continue;

				auto key = static_cast<Keys>(keyCode);
				auto finder = std::find(_keys.begin(), _keys.end(), key);

				if (finder == _keys.end())
					_keys.push_back(key);
			}
		}

		const auto capslock = 0x14;
		const auto numlock = 0x90;

		const auto clState = GetKeyState(capslock);
		const auto nlState = GetKeyState(numlock);

		return KeyboardState(_keys, static_cast<bool>(clState), static_cast<bool>(nlState));
	}
}