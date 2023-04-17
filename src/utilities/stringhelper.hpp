#ifndef XNA_HELPER_STRINGHELPER_HPP
#define XNA_HELPER_STRINGHELPER_HPP

#include <algorithm>
#include <string>

namespace xna {
	static constexpr void Replace(std::string& str, char oldChar, char newChar) {
		std::replace(str.begin(), str.end(), oldChar, newChar);
	}

    static constexpr void Replace(std::string& str, std::string const& from, std::string const& to) {
        //https://stackoverflow.com/questions/2896600/how-to-replace-all-occurrences-of-a-character-in-string
        size_t start_pos = 0;
        
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
    }

    static constexpr bool Contains(std::string const& strA, std::string const& strB) {
        //https://stackoverflow.com/questions/2340281/check-if-a-string-contains-a-string-in-c
        return strA.find(strB) != std::string::npos;
    }

    static constexpr bool StartWith(std::string const& str, std::string const& value) {
        //https://stackoverflow.com/questions/1878001/how-do-i-check-if-a-c-stdstring-starts-with-a-certain-string-and-convert-a
        return str.rfind(value, 0) == 0;
    }
}

#endif