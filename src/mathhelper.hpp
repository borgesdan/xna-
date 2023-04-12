#ifndef XNA_MATHHELPER_HPP
#define XNA_MATHHELPER_HPP

#include <cstdint>
#include <limits>
#include <cmath>
#include "csharp/numeric.hpp"

namespace xna {	
	class MathHelper {
	public:
		static constexpr double E = 2.7182818284590452354;
		static constexpr double PI = 3.14159265358979323846;
		static constexpr double Tau = 6.283185307179586476925;
		static constexpr double Epsilon = std::numeric_limits<double>::epsilon();		
		static constexpr double LOG10E = 0.4342945;
		static constexpr double LOG2E = 1.442695;
		static constexpr double PIOVER2 = PI / 2.0;
		static constexpr double PIOVER4 = PI / 4.0;
		static constexpr double TWOPI = PI * 2.0;

		//Math

		static constexpr double Abs(double value) {
			const cs::Ulong mask = 0x7FFFFFFFFFFFFFFF;
			cs::Ulong raw = DoubleToUInt64Bits(value);

			return UInt64BitsToDouble(raw & mask);
		}

		static constexpr cs::Int Sign(double value) {
			if (value < 0.0)
				return -1;
			else if (value > 0.0)
				return 1;
			else
				return 0;
		}

		//https://www.codeproject.com/Articles/69941/Best-Square-Root-Method-Algorithm-Function-Precisi
		//https://gist.github.com/alexshtf/eb5128b3e3e143187794
		static constexpr double Sqrt(double x) {
			return x >= 0 && x < std::numeric_limits<double>::infinity()
				? sqrtNewtonRaphson(x, x, 0)
				: std::numeric_limits<double>::quiet_NaN();
		}

		static constexpr cs::Ulong DoubleToUInt64Bits(double value) {
			return *((cs::Ulong*)&value);
		}

		static constexpr double UInt64BitsToDouble(cs::Ulong value) {
			return *((double*)&value);
		}

		//MathHelper

		static constexpr double ToDegrees(double radians) {
			return radians * 57.295779513082320876798154814105;
		}

		static constexpr double ToRadians(double degrees) {
			return degrees * (PI / 180.0);
		}

		static constexpr double Distance(double value1, double value2) {
			return Abs(value1 - value2);
		}

		static constexpr double Min(double value1, double value2) {
			return value1 < value2 ? value1 : value2;
		}

		template <typename T>
		static constexpr T Min(T value1, T value2) {
			return value1 < value2 ? value1 : value2;
		}

		static constexpr double Max(double value1, double value2) {
			return value1 > value2 ? value1 : value2;
		}

		template <typename T>
		static constexpr T Max(T value1, T value2) {
			return value1 > value2 ? value1 : value2;
		}
		
		static constexpr double Clamp(double value, double min, double max) {
			value = (value > max) ? max : value;
			value = (value < min) ? min : value;
			return value;
		}

		static constexpr double Lerp(double value1, double value2, double amount) {
			return value1 + (value2 - value1) * amount;
		}

		static constexpr double LerpPrecise(double value1, double value2, double amount) {
			return ((1.0 - amount) * value1) + (value2 * amount);
		}		

		static constexpr double Barycentric(double value1, double value2, double value3, double amount1, double amount2) {
			return value1 + (value2 - value1) * amount1 + (value3 - value1) * amount2;
		}

		static constexpr double SmoothStep(double value1, double value2, double amount) {
			const auto result = Clamp(amount, 0.0, 1.0);
			return Hermite(value1, 0.0, value2, 0.0, result);
		}

		static constexpr double CatmullRom(double value1, double value2, double value3, double value4, double amount) {
			auto amountSquared = amount * amount;
			auto amountCubed = amountSquared * amount;

			return (0.5 * (2.0 * value2 + (value3 - value1) * amount +
				(2.0 * value1 - 5.0 * value2 + 4.0 * value3 - value4) * amountSquared +
				(3.0 * value2 - value1 - 3.0 * value3 + value4) * amountCubed));
		}			

		static constexpr double Hermite(double value1, double tangent1, double value2, double tangent2, double amount) {
			double v1 = value1, v2 = value2, t1 = tangent1, t2 = tangent2, s = amount, result;
			double sCubed = s * s * s;
			double sSquared = s * s;

			if (amount == 0.0)
				result = value1;
			else if (amount == 1.0)
				result = value2;
			else
				result = (2 * v1 - 2 * v2 + t2 + t1) * sCubed +
				(3 * v2 - 3 * v1 - 2 * t1 - t2) * sSquared +
				t1 * s +
				v1;
			return result;
		}		

		static constexpr double WrapAngle(double angle) {
			const auto pi = PI;

			if ((angle > -pi) && (angle <= pi))
				return angle;

			angle = fmod(angle, TWOPI);

			if (angle <= -pi)
				return angle + TWOPI;

			if (angle > pi)
				return angle - TWOPI;

			return angle;
		}			

		static constexpr bool IsPowerOfTwo(cs::Int value) {
			return (value > 0) && ((value & (value - 1)) == 0);
		}

		static constexpr bool IsPositiveInfinity(double value) {
			return value == std::numeric_limits<double>::infinity();
		}

		private:
			static constexpr double sqrtNewtonRaphson(double x, double curr, double prev)	{
				return curr == prev
					? curr
					: sqrtNewtonRaphson(x, 0.5 * (curr + x / curr), curr);
			}
	};

	using Math = MathHelper;
}

#endif