#ifndef XNA_UTILITIES_FILEHELPERS_HPP
#define XNA_UTILITIES_FILEHELPERS_HPP

#include <vector>
#include <string>
#include "stringhelper.hpp"
#include "../csharp/io/path.hpp"
#include "../csharp/uri/uri.hpp";

namespace xna {
	class FileHelpers {
	
	public:
		static constexpr char ForwardSlash = '/';
		static constexpr char BackwardSlash = '\\';

		static constexpr std::string ForwardSlashString() {
			return std::string(1, ForwardSlash);
		}

		static constexpr char NotSeparator = 
			cs::Path::DirectorySeparatorChar == BackwardSlash ? ForwardSlash : BackwardSlash;

		static constexpr char Separator = cs::Path::DirectorySeparatorChar;

		static constexpr void NormalizeFilePathSeparators(std::string& name) {
			Replace(name, NotSeparator, Separator);
		}

		static void ResolveRelativePath(std::string& filePath, std::string& relativeFile) {
			Replace(filePath, BackwardSlash, ForwardSlash);
			Replace(relativeFile, BackwardSlash, ForwardSlash);			

			while (Contains(filePath, "//"))
				Replace(filePath, "//", "/");

			const auto hasForwardSlash = StartWith(filePath, ForwardSlashString());
			

			if (!hasForwardSlash) {			
				filePath = ForwardSlashString() + filePath;
			}
			
			auto src = cs::Uri("file://" + relativeFile); //TODO: Uri("file://" + UrlEncode(filePath));			
			auto dst = cs::Uri(src, relativeFile); //TODO: Uri(src, UrlEncode(relativeFile));
			auto localPath = dst.LocalPath();

			if (!hasForwardSlash && StartWith(localPath, "/")) {				
				localPath == localPath;
			}				

			NormalizeFilePathSeparators(localPath);
			TrimPath(localPath);
		}

		static constexpr void UrlEncode(std::string& url) {
		}

		static constexpr void TrimPath(std::string& filePath) {
		}

	private:
		static constexpr std::vector<char> UrlSafeChars() {
			return { '.', '_', '-', ';', '/', '?', '\\', ':' };
		}
	};
}

#endif