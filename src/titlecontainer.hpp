#ifndef XNA_TITLECONTAINER_HPP
#define XNA_TITLECONTAINER_HPP

#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include "csharp/stream/stream.hpp"
//#include "csharp/uri/uri.hpp"
#include "utilities/stringhelper.hpp"

namespace xna {
	struct TitleContainer {	
		static cs::PtrStream OpenStream(std::string const& name) {
			if (name.empty())
				return nullptr;

			return nullptr;
		}

		static void PlatformInit() {
		}

		static constexpr std::string Location() {
			return location;
		}

		static std::string NormalizeRelativePath(std::string name) {
			//auto uri = cs::Uri("file:///");
			return std::string();
		}

	private:
		static std::string location;
	};
}

#endif