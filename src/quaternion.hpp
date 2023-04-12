#ifndef XNA_QUATERNION_HPP
#define XNA_QUATERNION_HPP

#include <cmath>
#include "vector.hpp"
#include "matrix.hpp"
#include "mathhelper.hpp"

namespace xna {
	struct Quaternion {
		double X{ 0.0 };
		double Y{ 0.0 };
		double Z{ 0.0 };
		double W{ 0.0 };

		constexpr Quaternion() = default;

		constexpr Quaternion(const auto x, const auto y, const auto z, const auto w) :
			X(x), Y(y), Z(z), W(w) {}

		constexpr Quaternion(Vector3 const& vectorPart, double scalarPart) :
			X(vectorPart.X), Y(vectorPart.Y), Z(vectorPart.Z), W(scalarPart) {}

		static constexpr Quaternion Identity() {
			return Quaternion(0.0f, 0.0f, 0.0f, 1.0);
		}

		constexpr bool Equals(Quaternion const& other) const {
			return X == other.X
				&& Y == other.Y
				&& Z == other.Z
				&& W == other.W;
		}

		constexpr double LengthSquared() const {
			return (X * X + Y * Y + Z * Z + W * W);
		}

		constexpr double Length() const {
			return Math::Sqrt(X * X + Y * Y + Z * Z + W * W);
		}

		constexpr void Normalize() {
			const auto num = 1.0 / Math::Sqrt(X * X + Y * Y + Z * Z + W * W);
			X *= num;
			Y *= num;
			Z *= num;
			W *= num;
		}

		static constexpr Quaternion Normalize(Quaternion const& quaternion) {
			const auto num = 1.0 / Math::Sqrt(quaternion.X * quaternion.X + quaternion.Y * quaternion.Y + quaternion.Z * quaternion.Z + quaternion.W * quaternion.W);
			Quaternion quaternion1;
			quaternion1.X = quaternion.X * num;
			quaternion1.Y = quaternion.Y * num;
			quaternion1.Z = quaternion.Z * num;
			quaternion1.W = quaternion.W * num;
			return quaternion1;
		}

		void constexpr Conjugate() {
			X = -X;
			Y = -Y;
			Z = -Z;
		}

		static constexpr Quaternion Conjugate(Quaternion const& value) {
			Quaternion quaternion;
			quaternion.X = -value.X;
			quaternion.Y = -value.Y;
			quaternion.Z = -value.Z;
			quaternion.W = value.W;
			return quaternion;
		}

		static constexpr Quaternion Inverse(Quaternion const& quaternion) {
			const auto num = 1.0 / (quaternion.X * quaternion.X + quaternion.Y * quaternion.Y + quaternion.Z * quaternion.Z + quaternion.W * quaternion.W);
			Quaternion quaternion1;
			quaternion1.X = -quaternion.X * num;
			quaternion1.Y = -quaternion.Y * num;
			quaternion1.Z = -quaternion.Z * num;
			quaternion1.W = quaternion.W * num;
			return quaternion1;
		}

		static Quaternion CreateFromAxisAngle(Vector3 const& axis, double angle);
		static Quaternion CreateFromYawPitchRoll(double yaw, double pitch, double roll);
		static Quaternion CreateFromRotationMatrix(Matrix const& matrix);
		
		static constexpr double Dot(Quaternion const& quaternion1, Quaternion const& quaternion2) {
			return (quaternion1.X * quaternion2.X + quaternion1.Y * quaternion2.Y + quaternion1.Z * quaternion2.Z + quaternion1.W * quaternion2.W);
		}

		static Quaternion Slerp(Quaternion const& quaternion1, Quaternion const& quaternion2, double amount);
		static Quaternion Lerp(Quaternion const& quaternion1, Quaternion const& quaternion2, double amount);

		static constexpr Quaternion Concatenate(Quaternion const& value1, Quaternion const& value2) {
			const auto x1 = value2.X;
			const auto y1 = value2.Y;
			const auto z1 = value2.Z;
			const auto w1 = value2.W;
			const auto x2 = value1.X;
			const auto y2 = value1.Y;
			const auto z2 = value1.Z;
			const auto w2 = value1.W;
			const auto num1 = (y1 * z2 - z1 * y2);
			const auto num2 = (z1 * x2 - x1 * z2);
			const auto num3 = (x1 * y2 - y1 * x2);
			const auto num4 = (x1 * x2 + y1 * y2 + z1 * z2);
			Quaternion quaternion;
			quaternion.X = (x1 * w2 + x2 * w1) + num1;
			quaternion.Y = (y1 * w2 + y2 * w1) + num2;
			quaternion.Z = (z1 * w2 + z2 * w1) + num3;
			quaternion.W = w1 * w2 - num4;
			return quaternion;
		}

		static constexpr Quaternion Negate(Quaternion const& quaternion) {
			Quaternion quaternion1;
			quaternion1.X = -quaternion.X;
			quaternion1.Y = -quaternion.Y;
			quaternion1.Z = -quaternion.Z;
			quaternion1.W = -quaternion.W;
			return quaternion1;
		}

		static constexpr Quaternion Add(Quaternion const& quaternion1, Quaternion const& quaternion2) {
			Quaternion quaternion;
			quaternion.X = quaternion1.X + quaternion2.X;
			quaternion.Y = quaternion1.Y + quaternion2.Y;
			quaternion.Z = quaternion1.Z + quaternion2.Z;
			quaternion.W = quaternion1.W + quaternion2.W;
			return quaternion;
		}

		static constexpr Quaternion Subtract(Quaternion const& quaternion1, Quaternion const& quaternion2) {
			Quaternion quaternion;
			quaternion.X = quaternion1.X - quaternion2.X;
			quaternion.Y = quaternion1.Y - quaternion2.Y;
			quaternion.Z = quaternion1.Z - quaternion2.Z;
			quaternion.W = quaternion1.W - quaternion2.W;
			return quaternion;
		}

		static constexpr Quaternion Multiply(Quaternion const& quaternion1, Quaternion const& quaternion2) {
			const auto x1 = quaternion1.X;
			const auto y1 = quaternion1.Y;
			const auto z1 = quaternion1.Z;
			const auto w1 = quaternion1.W;
			const auto x2 = quaternion2.X;
			const auto y2 = quaternion2.Y;
			const auto z2 = quaternion2.Z;
			const auto w2 = quaternion2.W;
			const auto num1 = (y1 * z2 - z1 * y2);
			const auto num2 = (z1 * x2 - x1 * z2);
			const auto num3 = (x1 * y2 - y1 * x2);
			const auto num4 = (x1 * x2 + y1 * y2 + z1 * z2);
			Quaternion quaternion;
			quaternion.X = (x1 * w2 + x2 * w1) + num1;
			quaternion.Y = (y1 * w2 + y2 * w1) + num2;
			quaternion.Z = (z1 * w2 + z2 * w1) + num3;
			quaternion.W = w1 * w2 - num4;
			return quaternion;
		}

		static constexpr Quaternion Multiply(Quaternion const& quaternion1, double scaleFactor) {
			Quaternion quaternion;
			quaternion.X = quaternion1.X * scaleFactor;
			quaternion.Y = quaternion1.Y * scaleFactor;
			quaternion.Z = quaternion1.Z * scaleFactor;
			quaternion.W = quaternion1.W * scaleFactor;
			return quaternion;
		}

		static constexpr Quaternion Divide(Quaternion quaternion1, Quaternion quaternion2) {
			const auto x = quaternion1.X;
			const auto y = quaternion1.Y;
			const auto z = quaternion1.Z;
			const auto w = quaternion1.W;
			const auto num1 = 1.0 / (quaternion2.X * quaternion2.X + quaternion2.Y * quaternion2.Y + quaternion2.Z * quaternion2.Z + quaternion2.W * quaternion2.W);
			const auto num2 = -quaternion2.X * num1;
			const auto num3 = -quaternion2.Y * num1;
			const auto num4 = -quaternion2.Z * num1;
			const auto num5 = quaternion2.W * num1;
			const auto num6 = (y * num4 - z * num3);
			const auto num7 = (z * num2 - x * num4);
			const auto num8 = (x * num3 - y * num2);
			const auto num9 = (x * num2 + y * num3 + z * num4);
			Quaternion quaternion;
			quaternion.X = (x * num5 + num2 * w) + num6;
			quaternion.Y = (y * num5 + num3 * w) + num7;
			quaternion.Z = (z * num5 + num4 * w) + num8;
			quaternion.W = w * num5 - num9;
			return quaternion;
		}

		constexpr Quaternion operator -() {
			return Negate(*this);
		}

		friend constexpr bool operator ==(Quaternion const& quaternion1, Quaternion const& quaternion2) {
			return quaternion1.Equals(quaternion2);
		}

		friend constexpr bool operator !=(Quaternion const& quaternion1, Quaternion const& quaternion2) {
			return quaternion1.Equals(quaternion2);
		}

		friend constexpr Quaternion operator +(Quaternion const& quaternion1, Quaternion const& quaternion2) {
			return Quaternion::Add(quaternion1, quaternion2);
		}

		friend constexpr Quaternion operator -(Quaternion const& quaternion1, Quaternion const& quaternion2) {
			return Quaternion::Subtract(quaternion1, quaternion2);
		}

		friend constexpr Quaternion operator *(Quaternion const& quaternion1, Quaternion const& quaternion2) {
			return Quaternion::Multiply(quaternion1, quaternion2);
		}

		friend constexpr Quaternion operator *(Quaternion const& quaternion1, double scaleFactor) {
			return Quaternion::Multiply(quaternion1, scaleFactor);
		}

		friend constexpr Quaternion operator *(double scaleFactor, Quaternion const& quaternion2) {
			return Quaternion::Multiply(quaternion2, scaleFactor);
		}

		friend constexpr Quaternion operator /(Quaternion const& quaternion1, Quaternion const& quaternion2) {
			return Quaternion::Divide(quaternion1, quaternion2);
		}
	};
}

namespace xna {

	constexpr Vector2 Vector2::Transform(Vector2 const& value, Quaternion const& rotation) {
		const auto num1 = rotation.X + rotation.X;
		const auto num2 = rotation.Y + rotation.Y;
		const auto num3 = rotation.Z + rotation.Z;
		const auto num4 = rotation.W * num3;
		const auto num5 = rotation.X * num1;
		const auto num6 = rotation.X * num2;
		const auto num7 = rotation.Y * num2;
		const auto num8 = rotation.Z * num3;
		const auto num9 = value.X * (1.0 - num7 - num8) + value.Y * (num6 - num4);
		const auto num10 = value.X * (num6 + num4) + value.Y * (1.0 - num5 - num8);

		return Vector2(num9, num10);
	}

	constexpr Vector3 Vector3::Transform(Vector3 const& value, Quaternion const& rotation)
	{
		const auto num1 = rotation.X + rotation.X;
		const auto num2 = rotation.Y + rotation.Y;
		const auto num3 = rotation.Z + rotation.Z;
		const auto num4 = rotation.W * num1;
		const auto num5 = rotation.W * num2;
		const auto num6 = rotation.W * num3;
		const auto num7 = rotation.X * num1;
		const auto num8 = rotation.X * num2;
		const auto num9 = rotation.X * num3;
		const auto num10 = rotation.Y * num2;
		const auto num11 = rotation.Y * num3;
		const auto num12 = rotation.Z * num3;
		const auto num13 = (value.X * (1.0 - num10 - num12) + value.Y * (num8 - num6) + value.Z * (num9 + num5));
		const auto num14 = (value.X * (num8 + num6) + value.Y * (1.0 - num7 - num12) + value.Z * (num11 - num4));
		const auto num15 = (value.X * (num9 - num5) + value.Y * (num11 + num4) + value.Z * (1.0 - num7 - num10));

		Vector3 vector3;
		vector3.X = num13;
		vector3.Y = num14;
		vector3.Z = num15;

		return vector3;
	}

	constexpr Vector4 Vector4::Transform(Vector2 const& value, Quaternion const& rotation) {
		const auto num1 = rotation.X + rotation.X;
		const auto num2 = rotation.Y + rotation.Y;
		const auto num3 = rotation.Z + rotation.Z;
		const auto num4 = rotation.W * num1;
		const auto num5 = rotation.W * num2;
		const auto num6 = rotation.W * num3;
		const auto num7 = rotation.X * num1;
		const auto num8 = rotation.X * num2;
		const auto num9 = rotation.X * num3;
		const auto num10 = rotation.Y * num2;
		const auto num11 = rotation.Y * num3;
		const auto num12 = rotation.Z * num3;
		const auto num13 = (value.X * (1.0 - num10 - num12) + value.Y * (num8 - num6));
		const auto num14 = (value.X * (num8 + num6) + value.Y * (1.0 - num7 - num12));
		const auto num15 = (value.X * (num9 - num5) + value.Y * (num11 + num4));

		Vector4 vector4;
		vector4.X = num13;
		vector4.Y = num14;
		vector4.Z = num15;
		vector4.W = 1.0;
		return vector4;
	}

	constexpr Vector4 Vector4::Transform(Vector3 const& value, Quaternion const& rotation) {
		const auto num1 = rotation.X + rotation.X;
		const auto num2 = rotation.Y + rotation.Y;
		const auto num3 = rotation.Z + rotation.Z;
		const auto num4 = rotation.W * num1;
		const auto num5 = rotation.W * num2;
		const auto num6 = rotation.W * num3;
		const auto num7 = rotation.X * num1;
		const auto num8 = rotation.X * num2;
		const auto num9 = rotation.X * num3;
		const auto num10 = rotation.Y * num2;
		const auto num11 = rotation.Y * num3;
		const auto num12 = rotation.Z * num3;
		const auto num13 = (value.X * (1.0 - num10 - num12) + value.Y * (num8 - num6) + value.Z * (num9 + num5));
		const auto num14 = (value.X * (num8 + num6) + value.Y * (1.0 - num7 - num12) + value.Z * (num11 - num4));
		const auto num15 = (value.X * (num9 - num5) + value.Y * (num11 + num4) + value.Z * (1.0 - num7 - num10));
		Vector4 vector4;
		vector4.X = num13;
		vector4.Y = num14;
		vector4.Z = num15;
		vector4.W = 1.0;
		return vector4;
	}

	constexpr Vector4 Vector4::Transform(Vector4 const& value, Quaternion const& rotation) {
		const auto num1 = rotation.X + rotation.X;
		const auto num2 = rotation.Y + rotation.Y;
		const auto num3 = rotation.Z + rotation.Z;
		const auto num4 = rotation.W * num1;
		const auto num5 = rotation.W * num2;
		const auto num6 = rotation.W * num3;
		const auto num7 = rotation.X * num1;
		const auto num8 = rotation.X * num2;
		const auto num9 = rotation.X * num3;
		const auto num10 = rotation.Y * num2;
		const auto num11 = rotation.Y * num3;
		const auto num12 = rotation.Z * num3;
		const auto num13 = (value.X * (1.0 - num10 - num12) + value.Y * (num8 - num6) + value.Z * (num9 + num5));
		const auto num14 = (value.X * (num8 + num6) + value.Y * (1.0 - num7 - num12) + value.Z * (num11 - num4));
		const auto num15 = (value.X * (num9 - num5) + value.Y * (num11 + num4) + value.Z * (1.0 - num7 - num10));

		Vector4 vector4;
		vector4.X = num13;
		vector4.Y = num14;
		vector4.Z = num15;
		vector4.W = value.W;
		return vector4;
	}
}

namespace xna {
	constexpr Matrix Matrix::CreateFromQuaternion(Quaternion const& quaternion) {
		const auto num1 = quaternion.X * quaternion.X;
		const auto num2 = quaternion.Y * quaternion.Y;
		const auto num3 = quaternion.Z * quaternion.Z;
		const auto num4 = quaternion.X * quaternion.Y;
		const auto num5 = quaternion.Z * quaternion.W;
		const auto num6 = quaternion.Z * quaternion.X;
		const auto num7 = quaternion.Y * quaternion.W;
		const auto num8 = quaternion.Y * quaternion.Z;
		const auto num9 = quaternion.X * quaternion.W;
		Matrix fromQuaternion;
		fromQuaternion.M11 = (1.0 - 2.0 * (num2 + num3));
		fromQuaternion.M12 = (2.0 * (num4 + num5));
		fromQuaternion.M13 = (2.0 * (num6 - num7));
		fromQuaternion.M14 = 0.0;
		fromQuaternion.M21 = (2.0 * (num4 - num5));
		fromQuaternion.M22 = (1.0 - 2.0 * (num3 + num1));
		fromQuaternion.M23 = (2.0 * (num8 + num9));
		fromQuaternion.M24 = 0.0;
		fromQuaternion.M31 = (2.0 * (num6 + num7));
		fromQuaternion.M32 = (2.0 * (num8 - num9));
		fromQuaternion.M33 = (1.0 - 2.0 * (num2 + num1));
		fromQuaternion.M34 = 0.0;
		fromQuaternion.M41 = 0.0;
		fromQuaternion.M42 = 0.0;
		fromQuaternion.M43 = 0.0;
		fromQuaternion.M44 = 1.0;
		return fromQuaternion;
	}		

	constexpr Matrix Matrix::Transform(Matrix const& value, Quaternion const& rotation) {
		const auto num1 = rotation.X + rotation.X;
		const auto num2 = rotation.Y + rotation.Y;
		const auto num3 = rotation.Z + rotation.Z;
		const auto num4 = rotation.W * num1;
		const auto num5 = rotation.W * num2;
		const auto num6 = rotation.W * num3;
		const auto num7 = rotation.X * num1;
		const auto num8 = rotation.X * num2;
		const auto num9 = rotation.X * num3;
		const auto num10 = rotation.Y * num2;
		const auto num11 = rotation.Y * num3;
		const auto num12 = rotation.Z * num3;
		const auto num13 = 1.0 - num10 - num12;
		const auto num14 = num8 - num6;
		const auto num15 = num9 + num5;
		const auto num16 = num8 + num6;
		const auto num17 = 1.0 - num7 - num12;
		const auto num18 = num11 - num4;
		const auto num19 = num9 - num5;
		const auto num20 = num11 + num4;
		const auto num21 = 1.0 - num7 - num10;
		Matrix matrix;
		matrix.M11 = (value.M11 * num13 + value.M12 * num14 + value.M13 * num15);
		matrix.M12 = (value.M11 * num16 + value.M12 * num17 + value.M13 * num18);
		matrix.M13 = (value.M11 * num19 + value.M12 * num20 + value.M13 * num21);
		matrix.M14 = value.M14;
		matrix.M21 = (value.M21 * num13 + value.M22 * num14 + value.M23 * num15);
		matrix.M22 = (value.M21 * num16 + value.M22 * num17 + value.M23 * num18);
		matrix.M23 = (value.M21 * num19 + value.M22 * num20 + value.M23 * num21);
		matrix.M24 = value.M24;
		matrix.M31 = (value.M31 * num13 + value.M32 * num14 + value.M33 * num15);
		matrix.M32 = (value.M31 * num16 + value.M32 * num17 + value.M33 * num18);
		matrix.M33 = (value.M31 * num19 + value.M32 * num20 + value.M33 * num21);
		matrix.M34 = value.M34;
		matrix.M41 = (value.M41 * num13 + value.M42 * num14 + value.M43 * num15);
		matrix.M42 = (value.M41 * num16 + value.M42 * num17 + value.M43 * num18);
		matrix.M43 = (value.M41 * num19 + value.M42 * num20 + value.M43 * num21);
		matrix.M44 = value.M44;
		return matrix;
	}
}

#endif