#ifndef XNA_TITLECONTAINER_HPP
#define XNA_TITLECONTAINER_HPP

#include <string>
#include "csharp/stream/stream.hpp"
#include "csharp/uri/uri.hpp"
#include "utilities/stringhelper.hpp"
#include "utilities/filehelpers.hpp"

namespace xna {
	struct TitleContainer {
		static cs::PtrStream OpenStream(std::string const& name) {
			if (name.empty())
				return nullptr;

			if (cs::Path::IsPathRooted(name))
				return nullptr;

			auto safeName = name;
			NormalizeRelativePath(safeName);
			auto stream = PlatformOpenStream(safeName);

			return stream;
		}

		static cs::PtrStream PlatformOpenStream(std::string const& safeName) {
			auto absolutePath = cs::Path::Combine(location, safeName);
			//TODO: return File.OpenRead(absolutePath);
			return nullptr;
		}

		static void PlatformInit() {
			//TODO PlatformInit
			//https://stackoverflow.com/questions/143174/how-do-i-get-the-directory-that-a-program-is-running-from
		}

		static constexpr std::string Location() {
			return location;
		}

		static std::string NormalizeRelativePath(std::string& name) {
			FileHelpers::UrlEncode(name);
			auto uri = cs::Uri("file:///" + name);
			auto path = uri.LocalPath();
			auto subPath = path.substr(1);
			Replace(path, FileHelpers::NotSeparator, FileHelpers::Separator);

			return path;
		}

	private:
		static std::string location;
	};
}

#endif