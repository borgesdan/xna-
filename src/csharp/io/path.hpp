#ifndef CS_IO_PATH_HPP
#define CS_IO_PATH_HPP

#include <string>
#include <vector>
#include "../integralnumeric.hpp"
#include "../nullable.hpp"

namespace cs {
	class PathInternal {
	public:
		static constexpr char DirectorySeparatorChar = '/';
		static constexpr char AltDirectorySeparatorChar = '/';
		static constexpr char VolumeSeparatorChar = '/';
		static constexpr char PathSeparator = ':';
		static constexpr std::string DirectorySeparatorCharAsString() { return "/"; };
		static constexpr std::string ParentDirectoryPrefix() { return "../"; }
		static constexpr std::string DirectorySeparators() { return DirectorySeparatorCharAsString(); }
		static constexpr std::string Utf8DirectorySeparators() { return "/"; }

		static constexpr csint GetRootLength(std::string const& path) {
			return path.size() > 0 && IsDirectorySeparator(path[0]) ? 1 : 0;
		}

		static constexpr bool IsDirectorySeparator(char c) {
			return c == DirectorySeparatorChar;
		}

		static constexpr std::string NormalizeDirectorySeparators(std::string const& path) {
			if (path.empty())
				return path;

			bool normalized = true;

			for (size_t i = 0; i < path.size(); i++) {
				if (IsDirectorySeparator(path[i])
					&& (i + 1 < path.size() && IsDirectorySeparator(path[i + 1])))
				{
					normalized = false;
					break;
				}
			}

			if (normalized)
				return path;

			std::string builder;

			for (size_t i = 0; i < path.size(); i++) {
				char current = path[i];

				if (IsDirectorySeparator(current)
					&& (i + 1 < path.size() && IsDirectorySeparator(path[i + 1])))
					continue;

				builder += current;
			}

			return builder;
		}

		static constexpr bool IsPartiallyQualified(std::string const& path) {
			//return !Path.IsPathRooted(path);
			return false;
		}

		static constexpr bool IsEffectivelyEmpty(std::string const& path) {
			return path.empty();
		}

		static constexpr bool StartsWithDirectorySeparator(std::string const& path) {
			return path.size() > 0 && IsDirectorySeparator(path[0]);
		}

		static constexpr std::string EnsureTrailingSeparator(std::string const& path) {
			return EndsInDirectorySeparator(path)
				? path : path + DirectorySeparatorCharAsString();
		}

		static constexpr bool IsRoot(std::string const& path) {
			return path.size() == GetRootLength(path);
		}

		static constexpr csint GetCommonPathLength(std::string const& first, std::string const& second, bool ignoreCase) {
			csint commonChars = EqualStartingCharacterCount(first, second, ignoreCase);

			if (commonChars == 0)
				return commonChars;

			if (commonChars == first.size()
				&& (commonChars == second.size() || IsDirectorySeparator(second[commonChars])))
				return commonChars;

			if (commonChars == second.size() && IsDirectorySeparator(first[commonChars]))
				return commonChars;

			while (commonChars > 0 && !IsDirectorySeparator(first[commonChars - 1]))
				commonChars--;

			return commonChars;
		}

		static constexpr csint EqualStartingCharacterCount(std::string const& first, std::string const& second, bool ignoreCase) {
			if (first.empty() || second.empty())
				return 0;

			int commonChars = 0;
			const auto f = first.c_str();
			const auto s = second.c_str();

			auto l = f;
			auto r = s;
			auto leftEnd = l + first.size();
			auto rightEnd = r + second.size();

			/*while (l != leftEnd && r != rightEnd
				&& (*l == *r || (ignoreCase && char.ToUpperInvariant(*l) == char.ToUpperInvariant(*r))))
			{
				commonChars++;
				l++;
				r++;
			}*/

			return commonChars;
		}

		static constexpr bool EndsInDirectorySeparator(std::string const& path) {
			return path.size() > 0 && IsDirectorySeparator(path[path.size() - 1]);
		}
	};

	class Path {
	public:
		static constexpr std::string DefaultTempPath() { return "/tmp/"; };
		static constexpr char DirectorySeparatorChar = PathInternal::DirectorySeparatorChar;
		static constexpr char AltDirectorySeparatorChar = PathInternal::AltDirectorySeparatorChar;
		static constexpr char VolumeSeparatorChar = PathInternal::VolumeSeparatorChar;
		static constexpr char PathSeparator = PathInternal::PathSeparator;

		static constexpr std::vector<char> GetInvalidFileNameChars() {
			return std::vector<char>
			{
				'\0', '/'
			};
		}

		static constexpr std::vector<char> GetInvalidPathChars() {
			return std::vector<char>
			{
				'\0'
			};
		}

		static constexpr std::string ChangeExtension(std::string const& path) {
			auto subLength = path.size();

			if (subLength == 0)
				return std::string();

			for (size_t i = path.size() - 1; i >= 0; i--) {
				char ch = path[i];

				if (ch == '.') {
					subLength = i;
					break;
				}

				if (PathInternal::IsDirectorySeparator(ch)) {
					break;
				}
			}

			return path.substr(0, subLength);
		}

		static constexpr std::string ChangeExtension(std::string const& path, std::string const& extension) {
			auto subLength = path.size();

			if (subLength == 0)
				return std::string();

			for (size_t i = path.size() - 1; i >= 0; i--) {
				char ch = path[i];

				if (ch == '.') {
					subLength = i;
					break;
				}

				if (PathInternal::IsDirectorySeparator(ch)) {
					break;
				}
			}			

			std::string subpath = path.substr(0, subLength);
			
			return extension[0] == '.' ?
				subpath += extension :
				subpath += "." + extension;
		}

		static constexpr bool Exists(std::string const& path) {
			if (path.empty())
				return false;

			std::string fullPath = GetFullPath(path);

			if (fullPath.empty())
				return false;

			bool isDirectory = false;
			//bool result = ExistsCore(fullPath);
			bool result = false;

			if (result && PathInternal::IsDirectorySeparator(fullPath[fullPath.size() - 1])) {
				return isDirectory;
			}

			return result;
		}

		static constexpr cs::Nullable<std::string> GetDirectoryName(std::string const& path) {
			if (PathInternal::IsEffectivelyEmpty(path))
				return nullptr;

			//csint end = GetDirectoryNameOffset(path);
			csint end = 0;

			return end >= 0 ? PathInternal::NormalizeDirectorySeparators(path.substr(0, end)) : nullptr;
		}

		static constexpr csint GetDirectoryNameOffset(std::string const& path) {
			auto rootLength = PathInternal::GetRootLength(path);
			auto end = path.size();

			if (end <= rootLength)
				return -1;

			while (end > rootLength && !PathInternal::IsDirectorySeparator(path[--end]));
			
			while (end > rootLength && PathInternal::IsDirectorySeparator(path[end - 1]))
				end--;

			return end;
		}

		static constexpr std::string GetExtension(std::string const& path) {
			auto length = path.size();

			for (size_t i = length - 1; i >= 0; i--) {
				auto ch = path[i];
				
				if (ch == '.') {
					if (i != length - 1)
						//return path.Slice(i, length - i);
						return std::string();
					else
						return std::string();
				}
				if (PathInternal::IsDirectorySeparator(ch))
					break;
			}

			return std::string();
		}

		static constexpr std::string GetFileName(std::string const& path) {
			//auto root = GetPathRoot(path).size();			

			/*auto i = PathInternal::DirectorySeparatorChar == PathInternal::AltDirectorySeparatorChar ?
				path.LastIndexOf(PathInternal.DirectorySeparatorChar) :
				path.LastIndexOfAny(PathInternal.DirectorySeparatorChar, PathInternal.AltDirectorySeparatorChar);

			return path.Slice(i < root ? root : i + 1);*/
			return std::string();
		}

		static constexpr std::string GetFileNameWithoutExtension(std::string const& path) {
			//{
			//	ReadOnlySpan<char> fileName = GetFileName(path);
			//	int lastPeriod = fileName.LastIndexOf('.');
			//	return lastPeriod < 0 ?
			//		fileName : // No extension was found
			//		fileName.Slice(0, lastPeriod);
			return std::string();
		}

		static constexpr std::string GetRandomFileName() {
			return std::string();
		}

		static constexpr bool IsPathFullyQualified(std::string const& path) {
			return !PathInternal::IsPartiallyQualified(path);
		}

		static constexpr bool HasExtension(std::string const& path) {
			for (size_t i = path.size() - 1; i >= 0; i--) {
				auto ch = path[i];
				
				if (ch == '.') {
					return i != path.size() - 1;
				}

				if (PathInternal::IsDirectorySeparator(ch))
					break;
			}

			return false;
		}

		static constexpr std::string Combine(std::string const& path1, std::string const& path2) {
			return std::string();
		}

		static constexpr std::string Combine(std::string const& path1, std::string const& path2, std::string const& path3) {
			return std::string();
		}

		static constexpr std::string Combine(std::string const& path1, std::string const& path2,
			std::string const& path3, std::string const& path4) {
			return std::string();
		}

		static constexpr std::string Combine(std::initializer_list<std::string> paths) {
			return std::string();
		}

		static constexpr std::string GetFullPath(std::string const& path)
		{
			if (path.empty())
				return path;

			if (path.find('\0') != std::string::npos)
				return std::string();

			// return GetFullPathInternal(path);
			return std::string();
		}		

	private:
		static constexpr int KeyLength = 8;

		static constexpr std::string CombineInternal(std::string first, std::string second)	{
			if (first.empty())
				return second;

			if (second.empty())
				return first;

			return std::string();

			/*if (IsPathRooted(second))
				return second;

			return JoinInternal(first.AsSpan(), second.AsSpan());*/
		}
	};
}

#endif