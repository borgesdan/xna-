#ifndef CS_ENUMERATIONS_HPP
#define CS_ENUMERATIONS_HPP

namespace cs {
	enum class SeekOrigin {
		Begin = 0,
		Current = 1,
		End = 2
	};

	enum class FileAccess {
		Read = 1,
		Write = 2,
		ReadWrite = 3,
	};

	enum class FileMode {
		CreateNew = 1,
		Create = 2,
		Open = 3,
		OpenOrCreate = 4,
		Truncate = 5,
		Append = 6,
	};
}

#endif