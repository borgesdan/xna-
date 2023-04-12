#ifndef XNA_GRAPHICS_PACKEDVECTOR_HPP
#define XNA_GRAPHICS_PACKEDVECTOR_HPP

#include <cmath>
#include "../csharp/integralnumeric.hpp"
#include "../csharp/floatnumeric.hpp"
#include "../vector.hpp"

//Utils
namespace xna{
	struct PackUtils {

		static constexpr csuint PackUnsigned(double bitmask, double value) {
			return touint(ClampAndRound(value, 0.0, bitmask));
		}

		static constexpr csuint PackSigned(csuint bitmask, double value)
		{
			const auto max = todouble(bitmask >> 1);
			const auto min = -max - 1.0;

			const auto result = toint(ClampAndRound(value, min, max)) & bitmask;
			return result;
		}

		static constexpr csuint PackUNorm(double bitmask, double value) {
			value *= bitmask;
			return touint(ClampAndRound(value, 0.0, bitmask));
		}

		static constexpr double UnpackUNorm(csuint bitmask, csuint value) {
			value &= bitmask;
			return todouble(value) / todouble(bitmask);
		}		

		static constexpr csuint PackSNorm(csuint bitmask, double value) {
			const auto max = todouble(bitmask >> 1);
			value *= max;			
			return toint(ClampAndRound(value, -max, max)) & bitmask;
		}

		static constexpr double UnpackSNorm(csuint bitmask, csuint value) {
			const auto num1 = bitmask + 1U >> 1;
			const auto valueAsInt = toint(value);

			if ((valueAsInt & toint(num1)) != 0) {

				if ((valueAsInt & toint(bitmask)) == toint(num1))
					return -1.0;

				value |= ~bitmask;
			}
			else
				value &= bitmask;

			const auto num2 = todouble(bitmask >> 1);
			const auto result = value / num2;
		}

		static constexpr double ClampAndRound(double value, double min, double max) {
			if (double_isnan(value))
				return 0.0;

			if (double_isinfinity(value))
				return double_isneginfinity(value) ? min : max;

			if (value < min)
				return min;

			return value > max ? max : std::round(value);
		}
	};

	struct HalfUtils {
		
		//TODO rever operador >>> em C#
		//https://stackoverflow.com/questions/5529443/what-is-operation-in-c

		static constexpr csushort Pack(double value) {
			const auto num1 = *(csuint*)&value;
			const csuint num2 = (num1 & 2147483648U) >> 16;
			const csuint num3 = num1 & int_max;
			csushort num4 = 0;

			if (num3 > 1207955455U)
				num4 = toushort(num2 | short_max);

			else if (num3 < 947912704U)	{
				const csuint num5 = num3 & 8388607 | 8388608;
				const csint num6 = 113 - toint(num3 >> 23);
				const csuint num7 = num6 > 31 ? 0U : num5 >> num6;

				num4 = toushort(num2 
					| (num7 + 4095 + touint((num7 >> 13) & 1) >> 13));
			}
			else
				num4 = toushort(num2 | touint(num3 - 939524096 + 4095 + touint((num3 >> 13) & 1) >> 13));

			return num4;			
		}

		static constexpr double Unpack(csushort value)	{
			csuint num1 = 0;
			
			const auto valueAsInt = toint(value);

			if ((valueAsInt & -33792) == 0) {
				if ((valueAsInt & 1023) != 0) {
					csuint num2 = 4294967282;
					csuint num3 = 0;
					
					for (num3 = touint(value) & 1023U; (toint(num3) & 1024) == 0; num3 <<= 1)
						--num2;

					csuint num4 = num3 & 4294966271U;
					
					num1 = touint(
						(valueAsInt & 32768) << 16 
						| toint(num2) + toint(sbyte_max) << 23 
						| toint(num4) << 13);
				}
				else
					num1 = touint((toint(value) & 32768) << 16);
			}
			else
				num1 = touint(
					(valueAsInt & 32768) << 16 
					| (valueAsInt >> 10 & 31) - 15 + toint(sbyte_max) << 23 
					| (valueAsInt & 1023) << 13);
			
			return *(double*)&num1;
		}

	private:

		//Variáveis não usadas no código C#!

		static constexpr csint cFracBits = 10;
		static constexpr csint cExpBits = 5;
		static constexpr csint cSignBit = 15;
		static constexpr csuint cSignMask = 32768;
		static constexpr csuint cFracMask = 1023;
		static constexpr csint cExpBias = 15;
		static constexpr csuint cRoundBit = 4096;
		static constexpr csuint eMax = 16;
		static constexpr csint eMin = -14;
		static constexpr csuint wMaxNormal = 1207955455;
		static constexpr csuint wMinNormal = 947912704;
		static constexpr csuint BiasDiffo = 3355443200;
		static constexpr csint cFracBitsDiff = 13;
	};
}

//Interfaces
namespace xna {
	struct IPackedVector {
		virtual constexpr Vector4 ToVector4() const = 0;
		virtual constexpr void PackFromVector4(xna::Vector4 const& vector) = 0;
	};

	template <typename T>
	struct IPackedVectorT : public IPackedVector {

		virtual constexpr T PackedValue() const = 0;
		virtual constexpr void PackedValue(T const& value) = 0;
	};		
}

//Alpha8
namespace xna {
	struct Alpha8 : IPackedVectorT<csbyte> {

		constexpr Alpha8() = default;

		constexpr Alpha8(double alpha) :
			packedValue(PackHelper(alpha)) {
		}

		virtual constexpr csbyte PackedValue() const override {
			return packedValue;
		}

		virtual constexpr void PackedValue(csbyte const& value) override {
			packedValue = value;
		}

		virtual constexpr xna::Vector4 ToVector4() const override {
			return Vector4(0.0, 0.0, 0.0, ToAlpha());
		}

		virtual constexpr void PackFromVector4(xna::Vector4 const& vector) override {
			packedValue = PackHelper(vector.W);
		}

		constexpr double ToAlpha() const {
			return PackUtils::UnpackUNorm(
				static_cast<csuint>(byte_max),
				static_cast<csuint>(packedValue));
		}

		constexpr bool Equals(Alpha8 const& other) const {
			return packedValue == other.packedValue;
		}

		friend constexpr bool operator ==(Alpha8 const& a, Alpha8 const& b) {
			return a.Equals(b);
		}

		friend constexpr bool operator !=(Alpha8 const& a, Alpha8 const& b) {
			return !a.Equals(b);
		}

	private:
		csbyte packedValue{ 0 };

		static constexpr csbyte PackHelper(double alpha) {
			return static_cast<csbyte>(PackUtils::PackUNorm(
				static_cast<double>(byte_max), alpha));
		}
	};
}

//Bgr565
namespace xna {
	struct Bgr565 : IPackedVectorT<csushort> {
		constexpr Bgr565() = default;

		constexpr Bgr565(double x, double y, double z) :
			packedValue(PackHelper(x, y, z)) {
		}

		constexpr Bgr565(Vector3 const& vector):
			packedValue(PackHelper(vector.X, vector.Y, vector.Z)) {
		}

		virtual constexpr csushort PackedValue() const override {
			return packedValue;
		}

		virtual constexpr void PackedValue(csushort const& value) override {
			packedValue = value;
		}

		virtual constexpr xna::Vector4 ToVector4() const override {
			const auto vector3 = ToVector3();
			return Vector4(vector3, 1.0);
		}

		virtual constexpr void PackFromVector4(xna::Vector4 const& vector) override {
			packedValue = PackHelper(vector.X, vector.Y, vector.Z);
		}

		constexpr Vector3 ToVector3() const {
			Vector3 vector3;
			vector3.X = PackUtils::UnpackUNorm(31U, static_cast<csuint>(packedValue >> 11));
			vector3.Y = PackUtils::UnpackUNorm(63U, static_cast<csuint>(packedValue >> 5));
			vector3.Z = PackUtils::UnpackUNorm(31U, static_cast<csuint>(packedValue));
			return vector3;
		}

		constexpr bool Equals(Bgr565 const& other) const {
			return packedValue == other.packedValue;
		}

		friend constexpr bool operator ==(Bgr565 const& a, Bgr565 const& b) {
			return a.Equals(b);
		}

		friend constexpr bool operator !=(Bgr565 const& a, Bgr565 const& b) {
			return !a.Equals(b);
		}

	private:
		csushort packedValue{0};

		static constexpr csushort PackHelper(double x, double y, double z) {
			return static_cast<csushort>(
				PackUtils::PackUNorm(31, x) << 11
				| PackUtils::PackUNorm(63, y) << 5
				| PackUtils::PackUNorm(31, z));
		}
	};
}

//Bgra4444
namespace xna {
	struct Bgra4444 : IPackedVectorT<csushort> {
		constexpr Bgra4444() = default;

		constexpr Bgra4444(double x, double y, double z, double w) :
		packedValue(PackHelper(x, y, z, w)) {
		}

		constexpr Bgra4444(Vector4 vector) :
			packedValue(PackHelper(vector.X, vector.Y, vector.Z, vector.W)) {
		}

		virtual constexpr csushort PackedValue() const override {
			return packedValue;
		}

		virtual constexpr void PackedValue(csushort const& value) override {
			packedValue = value;
		}

		virtual constexpr xna::Vector4 ToVector4() const override {
			Vector4 vector4;
			vector4.X = PackUtils::UnpackUNorm(15U, static_cast<csuint>(packedValue) >> 8);
			vector4.Y = PackUtils::UnpackUNorm(15U, static_cast<csuint>(packedValue) >> 4);
			vector4.Z = PackUtils::UnpackUNorm(15U, static_cast<csuint>(packedValue));
			vector4.W = PackUtils::UnpackUNorm(15U, static_cast<csuint>(packedValue) >> 12);
			return vector4;
		}

		virtual constexpr void PackFromVector4(xna::Vector4 const& vector) override {
			packedValue = Bgra4444::PackHelper(vector.X, vector.Y, vector.Z, vector.W);
		}

		constexpr bool Equals(Bgra4444 other) const {
			return packedValue == other.packedValue;
		}

		friend constexpr bool operator ==(Bgra4444 const& a, Bgra4444 const& b) {
			return a.Equals(b);
		}

		friend constexpr bool operator !=(Bgra4444 const& a, Bgra4444 const& b) {
			return !a.Equals(b);
		}

	private:
		csushort packedValue{ 0 };

		static constexpr csushort PackHelper(double vectorX, double vectorY, double vectorZ, double vectorW) {
			return static_cast<csushort>(
				PackUtils::PackUNorm(15.0, vectorX) << 8 
				| PackUtils::PackUNorm(15.0, vectorY) << 4 
				| PackUtils::PackUNorm(15.0, vectorZ) 
				| PackUtils::PackUNorm(15.0, vectorW) << 12);
		}
	};
}

//Bgra5551
namespace xna {
	struct Bgra5551 : IPackedVectorT<csushort> {
		constexpr Bgra5551() = default;

		constexpr Bgra5551(double x, double y, double z, double w) :
			packedValue(PackHelper(x, y, z, w)) {
		}

		constexpr Bgra5551(Vector4 vector) :
			packedValue(PackHelper(vector.X, vector.Y, vector.Z, vector.W)) {
		}

		virtual constexpr csushort PackedValue() const override {
			return packedValue;
		}

		virtual constexpr void PackedValue(csushort const& value) override {
			packedValue = value;
		}

		virtual constexpr xna::Vector4 ToVector4() const override {
			Vector4 vector4;
			vector4.X = PackUtils::UnpackUNorm(31U, static_cast<csuint>(packedValue) >> 10);
			vector4.Y = PackUtils::UnpackUNorm(31U, static_cast<csuint>(packedValue) >> 5);
			vector4.Z = PackUtils::UnpackUNorm(31U, static_cast<csuint>(packedValue));
			vector4.W = PackUtils::UnpackUNorm(1U, static_cast<csuint>(packedValue) >> 15);
			return vector4;
		}

		virtual constexpr void PackFromVector4(xna::Vector4 const& vector) override {
			packedValue = Bgra5551::PackHelper(vector.X, vector.Y, vector.Z, vector.W);
		}

		constexpr bool Equals(Bgra5551 other) const {
			return packedValue == other.packedValue;
		}

		friend constexpr bool operator ==(Bgra5551 const& a, Bgra5551 const& b) {
			return a.Equals(b);
		}

		friend constexpr bool operator !=(Bgra5551 const& a, Bgra5551 const& b) {
			return !a.Equals(b);
		}

	private:
		csushort packedValue{ 0 };

		static constexpr csushort PackHelper(double vectorX, double vectorY, double vectorZ, double vectorW) {
			return static_cast<csushort>(
				PackUtils::PackUNorm(31.0, vectorX) << 10 
				| PackUtils::PackUNorm(31.0, vectorY) << 5 
				| PackUtils::PackUNorm(31.0, vectorZ) 
				| PackUtils::PackUNorm(1.0, vectorW) << 15);
		}
	};
}

//Byte4
namespace xna {
	struct Byte4 : IPackedVectorT<csuint> {
		constexpr Byte4() = default;

		constexpr Byte4(double x, double y, double z, double w) :
			packedValue(PackHelper(x, y, z, w)) {
		}

		constexpr Byte4(Vector4 vector) :
			packedValue(PackHelper(vector.X, vector.Y, vector.Z, vector.W)) {
		}

		virtual constexpr csuint PackedValue() const override {
			return packedValue;
		}

		virtual constexpr void PackedValue(csuint const& value) override {
			packedValue = value;
		}

		virtual constexpr xna::Vector4 ToVector4() const override {
			Vector4 vector4;
			vector4.X = todouble(packedValue & touint(byte_max));
			vector4.Y = todouble(packedValue >> 8 & touint(byte_max));
			vector4.Z = todouble(packedValue >> 16 & touint(byte_max));
			vector4.W = todouble(packedValue >> 24 & touint(byte_max));
			return vector4;
		}

		virtual constexpr void PackFromVector4(xna::Vector4 const& vector) override {
			packedValue = Byte4::PackHelper(vector.X, vector.Y, vector.Z, vector.W);
		}

		constexpr bool Equals(Byte4 other) const {
			return packedValue == other.packedValue;
		}

		friend constexpr bool operator ==(Byte4 const& a, Byte4 const& b) {
			return a.Equals(b);
		}

		friend constexpr bool operator !=(Byte4 const& a, Byte4 const& b) {
			return !a.Equals(b);
		}

	private:
		csushort packedValue{ 0 };

		static constexpr csushort PackHelper(double vectorX, double vectorY, double vectorZ, double vectorW) {
			return PackUtils::PackUnsigned(static_cast<double>(byte_max), vectorX)
				| PackUtils::PackUnsigned(static_cast<double>(byte_max), vectorY) << 8
				| PackUtils::PackUnsigned(static_cast<double>(byte_max), vectorZ) << 16
				| PackUtils::PackUnsigned(static_cast<double>(byte_max), vectorW) << 24;
		}
	};
}

//HalfSingle
namespace xna {
	struct HalfSingle : IPackedVectorT<csushort> {

		constexpr HalfSingle() = default;

		constexpr HalfSingle(double value) :
			packedValue(HalfUtils::Pack(value)) {
		}

		virtual constexpr csushort PackedValue() const override {
			return packedValue;
		}

		virtual constexpr void PackedValue(csushort const& value) override {
			packedValue = value;
		}

		virtual constexpr xna::Vector4 ToVector4() const override {
			return Vector4(ToSingle(), 0.0, 0.0, 1.0);
		}

		virtual constexpr void PackFromVector4(xna::Vector4 const& vector) override {
			packedValue = HalfUtils::Pack(vector.X);
		}

		constexpr double ToSingle() const {
			return HalfUtils::Unpack(packedValue);
		}

		constexpr bool Equals(HalfSingle other) const {
			return packedValue == other.packedValue;
		}

		friend constexpr bool operator ==(HalfSingle const& a, HalfSingle const& b) {
			return a.Equals(b);
		}

		friend constexpr bool operator !=(HalfSingle const& a, HalfSingle const& b) {
			return !a.Equals(b);
		}
	
	private:
		csushort packedValue{ 0 };
	};
}

//HalfVector2
namespace xna {
	struct HalfVector2 : IPackedVectorT<csuint> {
		constexpr HalfVector2() = default;

		constexpr HalfVector2(double x, double y) :
			packedValue(PackHelper(x, y)) {
		}

		constexpr HalfVector2(Vector2 const& vector) :
			packedValue(PackHelper(vector.X, vector.Y)) {
		}

		virtual constexpr csuint PackedValue() const override {
			return packedValue;
		}

		virtual constexpr void PackedValue(csuint const& value) override {
			packedValue = value;
		}

		virtual constexpr xna::Vector4 ToVector4() const override {
			Vector2 vector2 = ToVector2();
			return Vector4(vector2.X, vector2.Y, 0.0, 1.0);
		}

		virtual constexpr void PackFromVector4(xna::Vector4 const& vector) override {
			packedValue = PackHelper(vector.X, vector.Y);
		}

		constexpr Vector2 ToVector2() const {
			Vector2 vector2;
			vector2.X = HalfUtils::Unpack(toushort(packedValue));
			vector2.Y = HalfUtils::Unpack(toushort(packedValue >> 16));
			return vector2;
		}

		constexpr bool Equals(HalfVector2 other) const {
			return packedValue == other.packedValue;
		}

		friend constexpr bool operator ==(HalfVector2 const& a, HalfVector2 const& b) {
			return a.Equals(b);
		}

		friend constexpr bool operator !=(HalfVector2 const& a, HalfVector2 const& b) {
			return !a.Equals(b);
		}

	private:
		csuint packedValue{0};

		static constexpr csuint PackHelper(double vectorX, double vectorY) {
			return touint(HalfUtils::Pack(vectorX)) | touint(HalfUtils::Pack(vectorY)) << 16;
		}
	};
}

//NormalizedByte2
namespace xna {
	struct NormalizedByte2 : IPackedVectorT<csushort> {
		constexpr NormalizedByte2() = default;

		constexpr NormalizedByte2(double x, double y) :
			packedValue(PackHelper(x, y)) {
		}

		constexpr NormalizedByte2(Vector2 const& vector) :
			packedValue(PackHelper(vector.X, vector.Y)) {
		}

		virtual constexpr csushort PackedValue() const override {
			return packedValue;
		}

		virtual constexpr void PackedValue(csushort const& value) override {
			packedValue = value;
		}

		virtual constexpr xna::Vector4 ToVector4() const override {
			Vector2 vector2 = ToVector2();
			return Vector4(vector2.X, vector2.Y, 0.0, 1.0);
		}

		virtual constexpr void PackFromVector4(xna::Vector4 const& vector) override {
			packedValue = PackHelper(vector.X, vector.Y);
		}

		constexpr Vector2 ToVector2() const {
			Vector2 vector2;
			vector2.X = PackUtils::UnpackSNorm(touint(byte_max), touint(packedValue));
			vector2.Y = PackUtils::UnpackSNorm(touint(byte_max), touint(packedValue) >> 8);
			return vector2;
		}

		constexpr bool Equals(NormalizedByte2 other) const {
			return packedValue == other.packedValue;
		}

		friend constexpr bool operator ==(NormalizedByte2 const& a, NormalizedByte2 const& b) {
			return a.Equals(b);
		}

		friend constexpr bool operator !=(NormalizedByte2 const& a, NormalizedByte2 const& b) {
			return !a.Equals(b);
		}

	private:
		csushort packedValue{0};

		static constexpr csushort PackHelper(double vectorX, double vectorY) {
			return toushort((PackUtils::PackSNorm(touint(byte_max), vectorX) 
				| PackUtils::PackSNorm(touint(byte_max), vectorY) << 8));
		}
	};
}

//NormalizedByte4
namespace xna {
	struct NormalizedByte4 : IPackedVectorT<csuint> {
		constexpr NormalizedByte4() = default;

		constexpr NormalizedByte4(double x, double y, double z, double w) :
			packedValue(PackHelper(x, y, z, w)) {
		}

		constexpr NormalizedByte4(Vector4 const& vector) :
			packedValue(PackHelper(vector.X, vector.Y, vector.Z, vector.W)) {
		}

		virtual constexpr csuint PackedValue() const override {
			return packedValue;
		}

		virtual constexpr void PackedValue(csuint const& value) override {
			packedValue = value;
		}

		virtual constexpr xna::Vector4 ToVector4() const override {
			Vector4 vector4;
			vector4.X = PackUtils::UnpackSNorm(touint(byte_max), packedValue);
			vector4.Y = PackUtils::UnpackSNorm(touint(byte_max), packedValue >> 8);
			vector4.Z = PackUtils::UnpackSNorm(touint(byte_max), packedValue >> 16);
			vector4.W = PackUtils::UnpackSNorm(touint(byte_max), packedValue >> 24);
			return vector4;
		}

		virtual constexpr void PackFromVector4(xna::Vector4 const& vector) override {
			packedValue = NormalizedByte4::PackHelper(vector.X, vector.Y, vector.Z, vector.W);
		}		

		constexpr bool Equals(NormalizedByte4 other) const {
			return packedValue == other.packedValue;
		}

		friend constexpr bool operator ==(NormalizedByte4 const& a, NormalizedByte4 const& b) {
			return a.Equals(b);
		}

		friend constexpr bool operator !=(NormalizedByte4 const& a, NormalizedByte4 const& b) {
			return !a.Equals(b);
		}

	private:
		csuint packedValue{ 0 };

		static constexpr csuint PackHelper(double vectorX, double vectorY, double vectorZ, double vectorW) {
			return PackUtils::PackSNorm(
				touint(byte_max), vectorX) 
				| PackUtils::PackSNorm(touint(byte_max), vectorY) << 8 
				| PackUtils::PackSNorm(touint(byte_max), vectorZ) << 16 
				| PackUtils::PackSNorm(touint(byte_max), vectorW) << 24;
		}
	};
}

//NormalizedShort2
namespace xna {
	struct NormalizedShort2 : IPackedVectorT<csuint> {
		constexpr NormalizedShort2() = default;

		constexpr NormalizedShort2(double x, double y) :
			packedValue(PackHelper(x, y)) {
		}

		constexpr NormalizedShort2(Vector2 const& vector) :
			packedValue(PackHelper(vector.X, vector.Y)) {
		}

		virtual constexpr csuint PackedValue() const override {
			return packedValue;
		}

		virtual constexpr void PackedValue(csuint const& value) override {
			packedValue = value;
		}

		virtual constexpr xna::Vector4 ToVector4() const override {
			Vector2 vector2 = ToVector2();
			return Vector4(vector2.X, vector2.Y, 0.0, 1.0);
		}

		virtual constexpr void PackFromVector4(xna::Vector4 const& vector) override {
			packedValue = NormalizedShort2::PackHelper(vector.X, vector.Y);
		}

		constexpr Vector2 ToVector2() const {
			Vector2 vector2;
			vector2.X = PackUtils::UnpackSNorm(touint(ushort_max), packedValue);
			vector2.Y = PackUtils::UnpackSNorm(touint(ushort_max), packedValue >> 16);
			return vector2;
		}

		constexpr bool Equals(NormalizedShort2 other) const {
			return packedValue == other.packedValue;
		}

		friend constexpr bool operator ==(NormalizedShort2 const& a, NormalizedShort2 const& b) {
			return a.Equals(b);
		}

		friend constexpr bool operator !=(NormalizedShort2 const& a, NormalizedShort2 const& b) {
			return !a.Equals(b);
		}

	private:
		csuint packedValue{ 0 };

		static constexpr csuint PackHelper(double vectorX, double vectorY) {
			return PackUtils::PackSNorm(
				touint(ushort_max), vectorX) 
				| PackUtils::PackSNorm(touint(ushort_max), vectorY) << 16;
		}	
	};
}

//NormalizedShort4
namespace xna {
	struct NormalizedShort4 : IPackedVectorT<csulong> {
		constexpr NormalizedShort4() = default;

		constexpr NormalizedShort4(double x, double y, double z, double w) :
			packedValue(PackHelper(x, y, z, w)) {
		}

		constexpr NormalizedShort4(Vector4 const& vector) :
			packedValue(PackHelper(vector.X, vector.Y, vector.Z, vector.W)) {
		}

		virtual constexpr csulong PackedValue() const override {
			return packedValue;
		}

		virtual constexpr void PackedValue(csulong const& value) override {
			packedValue = value;
		}

		virtual constexpr xna::Vector4 ToVector4() const override {
			Vector4 vector4;
			vector4.X = PackUtils::UnpackSNorm(touint(ushort_max), touint(packedValue));
			vector4.Y = PackUtils::UnpackSNorm(touint(ushort_max), touint(packedValue >> 16));
			vector4.Z = PackUtils::UnpackSNorm(touint(ushort_max), touint(packedValue >> 32));
			vector4.W = PackUtils::UnpackSNorm(touint(ushort_max), touint(packedValue >> 48));
			return vector4;
		}

		virtual constexpr void PackFromVector4(xna::Vector4 const& vector) override {
			packedValue = PackHelper(vector.X, vector.Y, vector.Z, vector.W);
		}

		constexpr Vector2 ToVector2() const {
			Vector2 vector2;
			vector2.X = PackUtils::UnpackSNorm(touint(ushort_max), touint(packedValue));
			vector2.Y = PackUtils::UnpackSNorm(touint(ushort_max), touint(packedValue >> 16));
			return vector2;
		}

		constexpr bool Equals(NormalizedShort4 other) const {
			return packedValue == other.packedValue;
		}

		friend constexpr bool operator ==(NormalizedShort4 const& a, NormalizedShort4 const& b) {
			return a.Equals(b);
		}

		friend constexpr bool operator !=(NormalizedShort4 const& a, NormalizedShort4 const& b) {
			return !a.Equals(b);
		}

	private:
		csulong packedValue{ 0 };

		static constexpr csulong PackHelper(double vectorX, double vectorY, double vectorZ, double vectorW) {
			return toulong(PackUtils::PackSNorm(touint(ushort_max), vectorX)) 
				| toulong(PackUtils::PackSNorm(touint(ushort_max), vectorY)) << 16 
				| toulong(PackUtils::PackSNorm(touint(ushort_max), vectorZ)) << 32
				| toulong(PackUtils::PackSNorm(touint(ushort_max), vectorW)) << 48;
		}
	};
}

//Rg32
namespace xna {
	struct Rg32 : IPackedVectorT<csuint> {
		constexpr Rg32() = default;

		constexpr Rg32(double x, double y) :
			packedValue(PackHelper(x, y)) {
		}

		constexpr Rg32(Vector2 const& vector) :
			packedValue(PackHelper(vector.X, vector.Y)) {
		}

		virtual constexpr csuint PackedValue() const override {
			return packedValue;
		}

		virtual constexpr void PackedValue(csuint const& value) override {
			packedValue = value;
		}

		virtual constexpr xna::Vector4 ToVector4() const override {
			Vector2 vector2 = ToVector2();
			return Vector4(vector2.X, vector2.Y);
		}

		virtual constexpr void PackFromVector4(xna::Vector4 const& vector) override {
			packedValue = PackHelper(vector.X, vector.Y);
		}

		constexpr Vector2 ToVector2() const {
			Vector2 vector2;
			vector2.X = PackUtils::UnpackUNorm(touint(ushort_max), packedValue);
			vector2.Y = PackUtils::UnpackUNorm(touint(ushort_max), packedValue >> 16);
			return vector2;
		}

		constexpr bool Equals(Rg32 other) const {
			return packedValue == other.packedValue;
		}

		friend constexpr bool operator ==(Rg32 const& a, Rg32 const& b) {
			return a.Equals(b);
		}

		friend constexpr bool operator !=(Rg32 const& a, Rg32 const& b) {
			return !a.Equals(b);
		}

	private:
		csuint packedValue{ 0 };

		static constexpr csuint PackHelper(double vectorX, double vectorY) {
			return PackUtils::PackUNorm(todouble(ushort_max), vectorX) 
				| PackUtils::PackUNorm(todouble(ushort_max), vectorY) << 16;
		}
	};
}

//Rgba64
namespace xna {
	struct Rgba64 : IPackedVectorT<csulong> {
		constexpr Rgba64() = default;

		constexpr Rgba64(double x, double y, double z, double w) :
			packedValue(PackHelper(x, y, z, w)) {
		}

		constexpr Rgba64(Vector4 const& vector) :
			packedValue(PackHelper(vector.X, vector.Y, vector.X, vector.W)) {
		}

		virtual constexpr csulong PackedValue() const override {
			return packedValue;
		}

		virtual constexpr void PackedValue(csulong const& value) override {
			packedValue = value;
		}

		virtual constexpr xna::Vector4 ToVector4() const override {
			Vector4 vector4;
			vector4.X = PackUtils::UnpackUNorm(touint(ushort_max), touint(packedValue));
			vector4.Y = PackUtils::UnpackUNorm(touint(ushort_max), touint(packedValue >> 16));
			vector4.Z = PackUtils::UnpackUNorm(touint(ushort_max), touint(packedValue >> 32));
			vector4.W = PackUtils::UnpackUNorm(touint(ushort_max), touint(packedValue >> 48));
			return vector4;
		}

		virtual constexpr void PackFromVector4(xna::Vector4 const& vector) override {
			packedValue = PackHelper(vector.X, vector.Y, vector.Z, vector.W);
		}

		constexpr bool Equals(Rgba64 other) const {
			return packedValue == other.packedValue;
		}

		friend constexpr bool operator ==(Rgba64 const& a, Rgba64 const& b) {
			return a.Equals(b);
		}

		friend constexpr bool operator !=(Rgba64 const& a, Rgba64 const& b) {
			return !a.Equals(b);
		}

	private:
		csulong packedValue{ 0 };

		static constexpr csulong PackHelper(double vectorX, double vectorY, double vectorZ, double vectorW) {
			return toulong(PackUtils::PackUNorm(todouble(ushort_max), vectorX))
				| toulong(PackUtils::PackUNorm(todouble(ushort_max), vectorY)) << 16
				| toulong(PackUtils::PackUNorm(todouble(ushort_max), vectorZ)) << 32
				| toulong(PackUtils::PackUNorm(todouble(ushort_max), vectorW)) << 48;
		}
	};
}

//Rgba1010102
namespace xna {
	struct Rgba1010102 : IPackedVectorT<csuint> {
		constexpr Rgba1010102() = default;

		constexpr Rgba1010102(double x, double y, double z, double w) :
			packedValue(PackHelper(x, y, z, w)) {
		}

		constexpr Rgba1010102(Vector4 const& vector) :
			packedValue(PackHelper(vector.X, vector.Y, vector.X, vector.W)) {
		}

		virtual constexpr csuint PackedValue() const override {
			return packedValue;
		}

		virtual constexpr void PackedValue(csuint const& value) override {
			packedValue = value;
		}

		virtual constexpr xna::Vector4 ToVector4() const override {
			Vector4 vector4;
			vector4.X = PackUtils::UnpackUNorm(1023U, packedValue);
			vector4.Y = PackUtils::UnpackUNorm(1023U, packedValue >> 10);
			vector4.Z = PackUtils::UnpackUNorm(1023U, packedValue >> 20);
			vector4.W = PackUtils::UnpackUNorm(3U, packedValue >> 30);
			return vector4;
		}

		virtual constexpr void PackFromVector4(xna::Vector4 const& vector) override {
			packedValue = PackHelper(vector.X, vector.Y, vector.Z, vector.W);
		}

		constexpr bool Equals(Rgba1010102 other) const {
			return packedValue == other.packedValue;
		}

		friend constexpr bool operator ==(Rgba1010102 const& a, Rgba1010102 const& b) {
			return a.Equals(b);
		}

		friend constexpr bool operator !=(Rgba1010102 const& a, Rgba1010102 const& b) {
			return !a.Equals(b);
		}

	private:
		csuint packedValue{ 0 };

		static constexpr csuint PackHelper(double vectorX, double vectorY, double vectorZ, double vectorW) {
			return PackUtils::PackUNorm(1023.0, vectorX) 
				| PackUtils::PackUNorm(1023.0, vectorY) << 10 
				| PackUtils::PackUNorm(1023.0, vectorZ) << 20 
				| PackUtils::PackUNorm(3.0, vectorW) << 30;
		}
	};
}

//Short2
namespace xna {
	struct Short2 : IPackedVectorT<csuint> {
		constexpr Short2() = default;

		constexpr Short2(double x, double y) :
			packedValue(PackHelper(x, y)) {
		}

		constexpr Short2(Vector2 const& vector) :
			packedValue(PackHelper(vector.X, vector.Y)) {
		}

		virtual constexpr csuint PackedValue() const override {
			return packedValue;
		}

		virtual constexpr void PackedValue(csuint const& value) override {
			packedValue = value;
		}

		virtual constexpr Vector4 ToVector4() const override {
			Vector2 vector2 = ToVector2();
			return Vector4(vector2.X, vector2.Y, 0.0, 1.0);
		}

		virtual constexpr void PackFromVector4(xna::Vector4 const& vector) override {
			packedValue = PackHelper(vector.X, vector.Y);
		}

		constexpr Vector2 ToVector2() const	{
			Vector2 vector2;
			vector2.X = todouble(packedValue);
			vector2.Y = todouble(packedValue >> 16);
			return vector2;
		}


	private:
		csuint packedValue{ 0 };

		static constexpr csuint PackHelper(double vectorX, double vectorY) {
			return PackUtils::PackSigned(
				touint(ushort_max), vectorX)
				| PackUtils::PackSigned(
					touint(ushort_max), vectorY) << 16;
		}
	};
}

//Short4
namespace xna {
	struct Short4 : IPackedVectorT<csulong> {
		constexpr Short4() = default;

		constexpr Short4(double x, double y, double z, double w) :
			packedValue(PackHelper(x, y, z, w)) {
		}

		constexpr Short4(Vector4 const& vector) :
			packedValue(PackHelper(vector.X, vector.Y, vector.Z, vector.W)) {
		}

		virtual constexpr csulong PackedValue() const override {
			return packedValue;
		}

		virtual constexpr void PackedValue(csulong const& value) override {
			packedValue = value;
		}

		virtual constexpr Vector4 ToVector4() const override {
			Vector4 vector4;
			vector4.X = todouble(packedValue);
			vector4.Y = todouble(packedValue >> 16);
			vector4.Z = todouble(packedValue >> 32);
			vector4.W = todouble(packedValue >> 48);
			return vector4;
		}

		virtual constexpr void PackFromVector4(xna::Vector4 const& vector) override {
			packedValue = PackHelper(vector.X, vector.Y, vector.Z, vector.W);
		}		


	private:
		csulong packedValue{ 0 };

		static constexpr csulong PackHelper(double vectorX, double vectorY, double vectorZ, double vectorW) {
			return toulong(
				PackUtils::PackSigned(touint(ushort_max), vectorX)) 
				| toulong(PackUtils::PackSigned(touint(ushort_max), vectorY)) << 16 
				| toulong(PackUtils::PackSigned(touint(ushort_max), vectorZ)) << 32
				| toulong(PackUtils::PackSigned(touint(ushort_max), vectorW)) << 48;
		}
	};
}

#endif