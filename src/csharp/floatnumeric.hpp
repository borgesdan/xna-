#ifndef CS_FLOATINGNUMERIC_HPP
#define CS_FLOATINGNUMERIC_HPP

#include "numeric.hpp"

static constexpr float float_max = cs::Limits::FloatMax;
static constexpr float float_min = cs::Limits::FloatMin;

static constexpr double double_max = cs::Limits::DoubleMax;
static constexpr double double_min = cs::Limits::DoubleMax;

static constexpr bool float_isinfinity(float value) { return cs::Floating::IsInfinity(value); }
static constexpr bool double_isinfinity(double value) { return cs::Floating::IsInfinity(value); }

static constexpr bool float_isnan(float value) { return cs::Floating::IsNaN(value); }
static constexpr bool double_isnan(double value) { return cs::Floating::IsNaN(value); }

static constexpr bool float_isneginfinity(float value) { return cs::Floating::IsNaN(value); }
static constexpr bool double_isneginfinity(double value) { return cs::Floating::IsNaN(value); }

template <typename T>
static constexpr float tofloat(T value) { return cs::Convert::ToFloat(value); }

template <typename T>
static constexpr double todouble(T value) { return cs::Convert::ToDouble(value); }

#endif