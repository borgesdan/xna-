#ifndef XNA_TITLECONTAINER_HPP
#define XNA_TITLECONTAINER_HPP

#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include "csharp/stream/stream.hpp"
#include "helpers/stringhelper.hpp"

namespace xna {
	class TitleContainer {
	public:
		static std::shared_ptr<cs::Stream> OpenStream(std::string const& name) {
			if (name.empty())
				return nullptr;

			return nullptr;
		}

		static void PlatformInit() {
		}

		static std::string Location() {
			return std::string();
		}

		static std::string NormalizeRelativePath(std::string name) {
			return std::string();
		}
	};
}

#endif