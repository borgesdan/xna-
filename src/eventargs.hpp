#ifndef XNA_EVENTARGS_HPP
#define XNA_EVENTARGS_HPP

#include <vector>
#include <string>
#include "input/enumerations.hpp"

namespace xna {
	struct FileDropEventArgs {
		constexpr FileDropEventArgs() = default;

		constexpr FileDropEventArgs(std::vector<std::string> files) : Files(files) {
		}

		std::vector<std::string> Files;
	};

	struct InputKeyEventArgs {
		constexpr InputKeyEventArgs(Keys key = Keys::None) : key(key) {
		}

		Keys key;
	};

	struct TextInputEventArgs {
		constexpr TextInputEventArgs() = default;

		constexpr TextInputEventArgs(char character, Keys key = Keys::None) :
			Character(character), Key(key) {
		}

		char Character;
		Keys Key;
	};
}

#endif