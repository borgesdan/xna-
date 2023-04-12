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

		static bool IsPathAbsolute(std::string const& path) {
			return false;
		}

		static std::string GetCleanPath(std::string& path) {
			/*Replace(path, '/', '\\');
			Replace(path, "\\.\\", "\\");

			const std::string dot_double_bar = ".\\";

			while (path.starts_with(dot_double_bar))
				path = path.substr(dot_double_bar.size());

			const std::string double_bar_dot = "\\.";
			const auto dbd_size = double_bar_dot.size();

			while (path.ends_with(double_bar_dot))
				path = path.size() <= dbd_size 
				? "\\" 
				: path.substr(0, path.size() - dbd_size);

			csint position1 = 0;
			auto path_size = path.size();
			
			for (size_t startIndex = 1; startIndex < path_size; startIndex = TitleContainer::CollapseParentDirectory(path, position1, "\\..\\".Length))	{
				position1 = path.IndexOf("\\..\\", startIndex);
				if (position1 < 0)
					break;
			}

			if (path.EndsWith("\\.."))
			{
				int position2 = path.Length - "\\..".Length;
				if (position2 > 0)
					TitleContainer.CollapseParentDirectory(ref path, position2, "\\..".Length);
			}
			if (path == ".")
				path = string.Empty;
			return path;*/

			return std::string();
		}

	private:
		static std::vector<char> bacCharacters;

		static constexpr csint CollapseParentDirectory(std::string& path, csint position, csint removeLength) {
			//const auto startIndex = path.LastIndexOf('\\', position - 1) + 1;
			//path = path.Remove(startIndex, position - startIndex + removeLength);
			//return Math.Max(startIndex - 1, 1);

			return -1;
		}

		static constexpr bool IsCleanPathAbsolute(std::string path) {
			//return path.IndexOfAny(TitleContainer.badCharacters) >= 0 || path.StartsWith("\\") || path.StartsWith("..\\") || path.Contains("\\..\\") || path.EndsWith("\\..") || path == "..";
			return false;
		}
	};

	std::vector<char> TitleContainer::bacCharacters
	{
		':',
		'*',
		'?',
		'"',
		'<',
		'>',
		'|'
	};
}

#endif