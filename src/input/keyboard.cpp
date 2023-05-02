#include "keyboard.hpp"

namespace xna {
	std::vector<csbyte> Keyboard::_keyState = std::vector<csbyte>(256);
	std::vector<Keys> Keyboard::_keys = std::vector<Keys>(10);
}