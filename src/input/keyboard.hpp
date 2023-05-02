#ifndef XNA_KEYBOARD_HPP
#define XNA_KEYBOARD_HPP

#include <vector>
#include "../basic-structs.hpp"
#include "enumerations.hpp"

namespace xna {
	struct KeyboardState {
		constexpr KeyboardState() {}

		constexpr KeyboardState(std::vector<Keys> const& keys, bool capsLock = false, bool numLock = false) {
			_modifiers = tobyte(0 | (capsLock ? CapsLockModifier : 0) | (numLock ? NumLockModifier : 0));

			if (!keys.empty()) {
				const auto size = keys.size();
				for (size_t k = 0; k < size; ++k) {
					InternalSetKey(keys[k]);
				}
			}
		}

		constexpr bool CapsLock() const {
			return (_modifiers & CapsLockModifier) > 0;
		}

		constexpr bool NumLock() const {
			return (_modifiers & NumLockModifier) > 0;
		}

		constexpr bool IsKeyDown(Keys key) const {
			return InternalGetKey(key);
		}

		constexpr bool IsKeyUp(Keys key) const {
			return !InternalGetKey(key);
		}

		bool constexpr InternalGetKey(Keys key) const {
			csuint mask = touint(1) << ((toint(key)) & 0x1F);
			csuint element;

			switch ((toint(key)) >> 5)
			{
			case 0: element = _keys0; break;
			case 1: element = _keys1; break;
			case 2: element = _keys2; break;
			case 3: element = _keys3; break;
			case 4: element = _keys4; break;
			case 5: element = _keys5; break;
			case 6: element = _keys6; break;
			case 7: element = _keys7; break;
			default: element = 0; break;
			}

			return (element & mask) != 0;;
		}

		void constexpr InternalSetKey(Keys key) {
			csuint mask = touint(1) << ((toint(key)) & 0x1F);

			switch ((toint(key)) >> 5)
			{
			case 0: _keys0 |= mask; break;
			case 1: _keys1 |= mask; break;
			case 2: _keys2 |= mask; break;
			case 3: _keys3 |= mask; break;
			case 4: _keys4 |= mask; break;
			case 5: _keys5 |= mask; break;
			case 6: _keys6 |= mask; break;
			case 7: _keys7 |= mask; break;
			}
		}

		void constexpr InternalClearKey(Keys key)
		{
			csuint mask = touint(1) << ((toint(key)) & 0x1F);

			switch ((toint(key)) >> 5)
			{
			case 0: _keys0 &= ~mask; break;
			case 1: _keys1 &= ~mask; break;
			case 2: _keys2 &= ~mask; break;
			case 3: _keys3 &= ~mask; break;
			case 4: _keys4 &= ~mask; break;
			case 5: _keys5 &= ~mask; break;
			case 6: _keys6 &= ~mask; break;
			case 7: _keys7 &= ~mask; break;
			}
		}

		void constexpr InternalClearAllKeys()
		{
			_keys0 = 0;
			_keys1 = 0;
			_keys2 = 0;
			_keys3 = 0;
			_keys4 = 0;
			_keys5 = 0;
			_keys6 = 0;
			_keys7 = 0;
		}

		constexpr csint GetPressedKeyCount() const {
			const auto count = CountBits(_keys0) + CountBits(_keys1) + CountBits(_keys2) + CountBits(_keys3)
				+ CountBits(_keys4) + CountBits(_keys5) + CountBits(_keys6) + CountBits(_keys7);

			return toint(count);
		}

		constexpr std::vector<Keys> GetPressedKeys() {
			csuint count = CountBits(_keys0) + CountBits(_keys1) + CountBits(_keys2) + CountBits(_keys3)
				+ CountBits(_keys4) + CountBits(_keys5) + CountBits(_keys6) + CountBits(_keys7);

			if (count == 0)
				return std::vector<Keys>();

			std::vector<Keys> keys;
			csint index = 0;

			if (_keys0 != 0)
				index = AddKeysToArray(_keys0, 0 * 32, keys, index);
			if (_keys1 != 0)
				index = AddKeysToArray(_keys1, 1 * 32, keys, index);
			if (_keys2 != 0)
				index = AddKeysToArray(_keys2, 2 * 32, keys, index);
			if (_keys3 != 0)
				index = AddKeysToArray(_keys3, 3 * 32, keys, index);
			if (_keys4 != 0)
				index = AddKeysToArray(_keys4, 4 * 32, keys, index);
			if (_keys5 != 0)
				index = AddKeysToArray(_keys5, 5 * 32, keys, index);
			if (_keys6 != 0)
				index = AddKeysToArray(_keys6, 6 * 32, keys, index);
			if (_keys7 != 0)
				index = AddKeysToArray(_keys7, 7 * 32, keys, index);

			return keys;
		}

		constexpr void GetPressedKeys(std::vector<Keys>& keys) {
			const auto count = CountBits(_keys0) + CountBits(_keys1) + CountBits(_keys2) + CountBits(_keys3)
				+ CountBits(_keys4) + CountBits(_keys5) + CountBits(_keys6) + CountBits(_keys7);

			if (count > keys.size()) {
				return;
			}

			csint index = 0;
			if (_keys0 != 0 && index < keys.size())
				index = AddKeysToArray(_keys0, 0 * 32, keys, index);
			if (_keys1 != 0 && index < keys.size())
				index = AddKeysToArray(_keys1, 1 * 32, keys, index);
			if (_keys2 != 0 && index < keys.size())
				index = AddKeysToArray(_keys2, 2 * 32, keys, index);
			if (_keys3 != 0 && index < keys.size())
				index = AddKeysToArray(_keys3, 3 * 32, keys, index);
			if (_keys4 != 0 && index < keys.size())
				index = AddKeysToArray(_keys4, 4 * 32, keys, index);
			if (_keys5 != 0 && index < keys.size())
				index = AddKeysToArray(_keys5, 5 * 32, keys, index);
			if (_keys6 != 0 && index < keys.size())
				index = AddKeysToArray(_keys6, 6 * 32, keys, index);
			if (_keys7 != 0 && index < keys.size())
				index = AddKeysToArray(_keys7, 7 * 32, keys, index);
		}

		constexpr bool Equals(KeyboardState const& other) const {
			return _keys0 == other._keys0
				&& _keys1 == other._keys1
				&& _keys2 == other._keys2
				&& _keys3 == other._keys3
				&& _keys4 == other._keys4
				&& _keys5 == other._keys5
				&& _keys6 == other._keys6
				&& _keys7 == other._keys7;
		}

		constexpr KeyState operator[](Keys key) const {
			return InternalGetKey(key) ? KeyState::Down : KeyState::Up;
		}

		friend constexpr bool operator ==(KeyboardState const& a, KeyboardState const& b) {
			return a.Equals(b);
		}

		friend constexpr bool operator !=(KeyboardState const& a, KeyboardState const& b) {
			return !a.Equals(b);
		}

	private:
		static constexpr csbyte CapsLockModifier = 1;
		static constexpr csbyte NumLockModifier = 2;

		csuint _keys0{ 0 };
		csuint _keys1{ 0 };
		csuint _keys2{ 0 };
		csuint _keys3{ 0 };
		csuint _keys4{ 0 };
		csuint _keys5{ 0 };
		csuint _keys6{ 0 };
		csuint _keys7{ 0 };
		csbyte _modifiers{ 0 };

		static constexpr csuint CountBits(csuint v) {
			v = v - ((v >> 1) & 0x55555555);
			v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
			return ((v + (v >> 4) & 0xF0F0F0F) * 0x1010101) >> 24;
		}

		static constexpr csint AddKeysToArray(csuint keys, csint offset, std::vector<Keys> pressedKeys, csint index) {
			for (size_t i = 0; i < 32; i++) {
				if ((keys & (1 << i)) != 0)
					pressedKeys[index++] = static_cast<Keys>(offset + i);
			}

			return index;
		}
	};
}

namespace xna {
	struct Keyboard {
		static KeyboardState GetState() {
			return KeyboardState();
		}

		static bool GetKeyboardState(csbyte* lpKeyState);

	private:
		static std::vector<csbyte> DefinedKeyCodes;
		static std::vector<csbyte> _keyState;
		static std::vector<Keys> _keys;
		static bool _isActive;
	};
}

#endif