#ifndef CS_FILE_HPP
#define CS_FILE_HPP

#include "../stream/stream.hpp"

namespace cs {
	struct File {
		static PtrFileStream Create(std::string const& path) {
			return FileStream::Make(path);
		}

		static PtrFileStream Open(std::string const& path) {
			return FileStream::Make(path);
		}
	};
}

#endif