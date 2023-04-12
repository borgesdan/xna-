#ifndef CS_INTEGRALNUMERIC_HPP
#define CS_INTEGRALNUMERIC_HPP

#include "Numeric.hpp"

using cssbyte = cs::Sbyte;
using csbyte = cs::Byte;
using csshort = cs::Short;
using csushort = cs::Ushort;
using csint = cs::Int;
using csuint = cs::Uint;
using cslong = cs::Long;
using csulong = cs::Ulong;

static constexpr cssbyte sbyte_max = cs::Limits::SbyteMax;
static constexpr csbyte byte_max = cs::Limits::ByteMax;
static constexpr csshort short_max = cs::Limits::ShortMax;
static constexpr csshort ushort_max = cs::Limits::UshortMax;
static constexpr csint int_max = cs::Limits::IntMax;
static constexpr csuint uint_max = cs::Limits::UintMax;
static constexpr cslong long_max = cs::Limits::LongMax;
static constexpr csulong ulong_max = cs::Limits::UlongMax;

static constexpr cssbyte sbyte_min = cs::Limits::SbyteMin;
static constexpr csbyte byte_min = cs::Limits::ByteMin;
static constexpr csshort short_min = cs::Limits::ShortMin;
static constexpr csshort ushort_min = cs::Limits::UshortMin;
static constexpr csint int_min = cs::Limits::IntMin;
static constexpr csuint uint_min = cs::Limits::UintMin;
static constexpr cslong long_min = cs::Limits::LongMin;
static constexpr csulong ulong_min = cs::Limits::UlongMin;

template <typename T>
static constexpr csint toint(T value) { return cs::Convert::ToInt(value); }

template <typename T>
static constexpr csuint touint(T value) { return cs::Convert::ToUint(value); }

template <typename T>
static constexpr cslong tolong(T value) { return cs::Convert::ToLong(value); }

template <typename T>
static constexpr csulong toulong(T value) { return cs::Convert::ToUlong(value); }

template <typename T>
static constexpr cssbyte tosbyte(T value) { return cs::Convert::ToSbyte(value); }

template <typename T>
static constexpr csbyte tobyte(T value) { return cs::Convert::ToByte(value); }

template <typename T>
static constexpr csshort toshort(T value) { return cs::Convert::ToShort(value); }

template <typename T>
static constexpr csushort toushort(T value) { return cs::Convert::ToUshort(value); }

#endif