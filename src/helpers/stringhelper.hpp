#ifndef XNA_HELPER_STRINGHELPER_HPP
#define XNA_HELPER_STRINGHELPER_HPP

#include <algorithm>
#include <string>

namespace xna {

    //https://stackoverflow.com/questions/2896600/how-to-replace-all-occurrences-of-a-character-in-string

	static constexpr void Replace(std::string& str, char oldChar, char newChar) {
		std::replace(str.begin(), str.end(), oldChar, newChar);
	}

    static constexpr void Replace(std::string& str, std::string const& from, std::string const& to) {
        size_t start_pos = 0;
        
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
    }
}

#endif