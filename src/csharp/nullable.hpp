#ifndef CS_NULLABLE_HPP
#define CS_NULLABLE_HPP

#include "numeric.hpp"

namespace cs {

	struct NullableBase {
		constexpr NullableBase() = default;

		constexpr bool HasValue() const {
			return _hasValue;
		}

	protected:
		bool _hasValue{ false };
	};

	template <typename T>
	struct Nullable : NullableBase {
		constexpr Nullable() = default;

		constexpr Nullable(NullableBase const& nullable) {}

		constexpr Nullable(T const& value) :
			_value(value) {
			_hasValue = true;
		}

		constexpr Nullable(T* const& value) {
			if (value) {
				_value = *value;
				_hasValue = true;
			}			
		}

		constexpr bool HasValue() const {
			return _hasValue;
		}

		constexpr T Value() const {
			return _value;
		}

	private:
		T _value = T();
	};

	static constexpr NullableBase csnull = NullableBase();

	using n_float = Nullable<float>;
	using n_double = Nullable<double>;
	using n_sbyte = Nullable<Sbyte>;
	using n_byte = Nullable<Byte>;
	using n_short = Nullable<Short>;
	using n_ushort = Nullable<Ushort>;
	using n_int = Nullable<Int>;
	using n_uint = Nullable<Uint>;
	using n_long = Nullable<Long>;
	using n_ulong = Nullable<Ulong>;
}

#endif