#ifndef XNA_VECTOR_HPP
#define XNA_VECTOR_HPP

#include <vector>
#include <cmath>
#include "mathhelper.hpp"

//Vector2
namespace xna {

	struct Matrix;
	struct Quaternion;

	struct Vector2 {
		double X{ 0.0 };
		double Y{ 0.0 };

		static constexpr Vector2 Zero() {
			return Vector2();
		}

		static constexpr Vector2 One() {
			return Vector2(1.0);
		}

		static constexpr Vector2 UnitX() {
			return Vector2(1.0, 0.0);
		}

		static constexpr Vector2 UnitY() {
			return Vector2(0.0, 1.0);
		}

		constexpr Vector2() = default;

		constexpr Vector2(double x, double y) :
			X(x), Y(y) {}

		constexpr Vector2(double value) :
			X(value), Y(value) {}

		static constexpr double Distance(Vector2 const& value1, Vector2 const& value2) {
			const auto num1 = value1.X - value2.X;
			const auto num2 = value1.Y - value2.Y;
			return Math::Sqrt(num1 * num1 + num2 * num2);
		}

		static constexpr double DistanceSquared(Vector2 const& value1, Vector2 const& value2) {
			const auto num1 = value1.X - value2.X;
			const auto num2 = value1.Y - value2.Y;
			return num1 * num1 + num2 * num2;
		}

		static constexpr double Dot(Vector2 const& value1, Vector2 const& value2) {
			return value1.X * value2.X + value1.Y * value2.Y;
		}

		static constexpr Vector2 Normalize(Vector2 const& value) {
			const auto num = 1. / Math::Sqrt(value.X * value.X + value.Y * value.Y);
			return Vector2(value.X * num, value.Y * num);
		}

		static constexpr Vector2 Reflect(Vector2 const& vector, Vector2 const& normal) {
			const auto num = vector.X * normal.X + vector.Y * normal.Y;
			return Vector2(vector.X - 2. * num * normal.X, vector.Y - 2. * num * normal.Y);
		}

		static constexpr Vector2 Min(Vector2 const& value1, Vector2 const& value2) {
			return Vector2(
				value1.X < value2.X ? value1.X : value2.X,
				value1.Y < value2.Y ? value1.Y : value2.Y);
		}

		static constexpr Vector2 Max(Vector2 const& value1, Vector2 const& value2) {
			return Vector2(
				value1.X > value2.X ? value1.X : value2.X,
				value1.Y > value2.Y ? value1.Y : value2.Y);
		}

		static constexpr Vector2 Clamp(Vector2 const& value1, Vector2 const& min, Vector2 const& max) {
			const auto x = value1.X;
			const auto num1 = x > max.X ? max.X : x;
			const auto num2 = num1 < min.X ? min.X : num1;

			const auto y = value1.Y;
			const auto num3 = y > max.Y ? max.Y : y;
			const auto num4 = num3 < min.Y ? min.Y : num3;

			return Vector2(num2, num4);
		}

		static constexpr Vector2 Lerp(Vector2 const& value1, Vector2 const& value2, double amount) {
			return Vector2(
				value1.X + (value2.X - value1.X) * amount,
				value1.Y + (value2.Y - value1.Y) * amount);
		}

		static constexpr Vector2 Barycentric(Vector2 const& value1, Vector2 const& value2,
			Vector2 const& value3, double amount1, double amount2) {
			return Vector2(
				value1.X + amount1 * (value2.X - value1.X) + amount2 * (value3.X - value1.X),
				value1.Y + amount1 * (value2.Y - value1.Y) + amount2 * (value3.Y - value1.Y));
		}

		static constexpr Vector2 SmoothStep(Vector2 const& value1, Vector2 const& value2, double amount) {
			amount = amount > 1.0 ? 1 : (amount < 0.0 ? 0.0 : amount);
			amount = amount * amount * (3.0 - 2.0 * amount);

			return Vector2(
				value1.X + (value2.X - value1.X) * amount,
				value1.Y + (value2.Y - value1.Y) * amount);
		}

		static constexpr Vector2 CatmullRom(Vector2 const& value1, Vector2 const& value2,
			Vector2 const& value3, Vector2 const& value4, double amount) {
			const auto num1 = amount * amount;
			const auto num2 = amount * num1;

			return Vector2(
				0.5 * (2.0 * value2.X + (-value1.X + value3.X) * amount + (2.0 * value1.X - 5.0 * value2.X + 4.0 * value3.X - value4.X) * num1 + (-value1.X + 3.0 * value2.X - 3.0 * value3.X + value4.X) * num2),
				0.5 * (2.0 * value2.Y + (-value1.Y + value3.Y) * amount + (2.0 * value1.Y - 5.0 * value2.Y + 4.0 * value3.Y - value4.Y) * num1 + (-value1.Y + 3.0 * value2.Y - 3.0 * value3.Y + value4.Y) * num2));
		}

		static constexpr Vector2 Hermite(Vector2 const& value1, Vector2 const& tangent1,
			Vector2 const& value2, Vector2 const& tangent2, double amount) {
			const auto num1 = amount * amount;
			const auto num2 = amount * num1;
			const auto num3 = 2.0 * num2 - 3.0 * num1 + 1.0;
			const auto num4 = -2.0 * num2 + 3.0 * num1;
			const auto num5 = num2 - 2.0 * num1 + amount;
			const auto num6 = num2 - num1;

			return Vector2(
				value1.X * num3 + value2.X * num4 + tangent1.X * num5 + tangent2.X * num6,
				value1.Y * num3 + value2.Y * num4 + tangent1.Y * num5 + tangent2.Y * num6);
		}

		static constexpr Vector2 Negate(Vector2 const& value) {
			Vector2 Vector2;
			Vector2.X = -value.X;
			Vector2.Y = -value.Y;
			return Vector2;
		}

		static constexpr Vector2 Add(Vector2 const& value1, Vector2 const& value2) {
			Vector2 Vector2;
			Vector2.X = value1.X + value2.X;
			Vector2.Y = value1.Y + value2.Y;
			return Vector2;
		}

		static constexpr Vector2 Subtract(Vector2 const& value1, Vector2 const& value2) {
			Vector2 Vector2;
			Vector2.X = value1.X - value2.X;
			Vector2.Y = value1.Y - value2.Y;
			return Vector2;
		}

		static constexpr Vector2 Multiply(Vector2 const& value1, Vector2 const& value2) {
			Vector2 Vector2;
			Vector2.X = value1.X * value2.X;
			Vector2.Y = value1.Y * value2.Y;
			return Vector2;
		}

		static constexpr Vector2 Multiply(Vector2 const& value1, double scaleFactor) {
			Vector2 Vector2;
			Vector2.X = value1.X * scaleFactor;
			Vector2.Y = value1.Y * scaleFactor;
			return Vector2;
		}

		static constexpr Vector2 Divide(Vector2 const& value1, Vector2 const& value2) {
			Vector2 Vector2;
			Vector2.X = value1.X / value2.X;
			Vector2.Y = value1.Y / value2.Y;
			return Vector2;
		}

		static constexpr Vector2 Divide(Vector2 const& value1, double value2) {
			const auto num = 1.0 / value2;
			Vector2 Vector2;
			Vector2.X = value1.X * num;
			Vector2.Y = value1.Y * num;
			return Vector2;
		}

		constexpr bool Equals(Vector2 const& other) const {
			return X == other.X && Y == other.Y;
		}

		constexpr void Normalize() {
			const auto num = 1. / Math::Sqrt(X * X + Y * Y);
			X *= num;
			Y *= num;
		}

		constexpr double Length() const {
			return Math::Sqrt(X * X + Y * Y);
		}

		constexpr double LengthSquared() const {
			return X * X + Y * Y;
		}

		constexpr Vector2 operator-() {
			return Negate(*this);
		}

		friend constexpr bool operator==(Vector2 const& value1, Vector2 const& value2) {
			return value1.Equals(value2);
		}

		friend constexpr bool operator!=(Vector2 const& value1, Vector2 const& value2) {
			return !value1.Equals(value2);
		}

		friend constexpr Vector2 operator+(Vector2 const& value1, Vector2 const& value2) {
			return Vector2::Add(value1, value2);
		}

		friend constexpr Vector2 operator-(Vector2 const& value1, Vector2 const& value2) {
			return Vector2::Subtract(value1, value2);
		}

		friend constexpr Vector2 operator*(Vector2 const& value1, Vector2 const& value2) {
			return Vector2::Multiply(value1, value2);
		}

		friend constexpr Vector2 operator*(Vector2 const& value, double scaleFactor) {
			return Vector2::Multiply(value, scaleFactor);
		}

		friend constexpr Vector2 operator*(double scaleFactor, Vector2 const& value) {
			return Vector2::Multiply(value, scaleFactor);
		}

		friend constexpr Vector2 operator/(Vector2 const& value1, Vector2 const& value2) {
			return Vector2::Divide(value1, value2);
		}

		friend constexpr Vector2 operator/(Vector2 const& value, double divider) {
			return Vector2::Divide(value, divider);
		}

		//Implementado em matrix.hpp

		static constexpr Vector2 Transform(Vector2 const& position, Matrix const& matrix);
		static constexpr Vector2 TransformNormal(Vector2 const& normal, Matrix const& matrix);
		static void Transform(std::vector<Vector2> const& sourceArray, Matrix const& matrix, std::vector<Vector2>& destinationArray);
		static void TransformNormal(std::vector<Vector2> const& sourceArray, Matrix const& matrix, std::vector<Vector2>& destinationArray);
		static void Transform(std::vector<Vector2> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector2>& destinationArray, size_t destinationIndex, size_t length);
		static void TransformNormal(std::vector<Vector2> const& sourceArray, size_t sourceIndex, Matrix& matrix, std::vector<Vector2> destinationArray, size_t destinationIndex, size_t length);

		//Implementado em quaternion.hpp

		static constexpr Vector2 Transform(Vector2 const& value, Quaternion const& rotation);
		static void Transform(std::vector<Vector2> const& sourceArray, Quaternion const& rotation, std::vector<Vector2>& destinationArray);
		static void Transform(std::vector<Vector2> const& sourceArray, size_t sourceIndex, Quaternion const& rotation, std::vector<Vector2>& destinationArray, size_t destinationIndex, size_t length);
	};

	using Vec2 = Vector2;
}

//Vector3
namespace xna {
	struct Matrix;
	struct Quaternion;

	struct Vector3 {
		double X{ 0.0 };
		double Y{ 0.0 };
		double Z{ 0.0 };

		static constexpr Vector3 Zero() { return Vector3(); }
		static constexpr Vector3 One() { return Vector3(1.0); }
		static constexpr Vector3 UnitX() { return Vector3(1.0, 0.0, 0.0); }
		static constexpr Vector3 UnitY() { return Vector3(0.0, 1.0, 0.0); }
		static constexpr Vector3 UnitZ() { return Vector3(0.0, 0.0, 1.0); }
		static constexpr Vector3 Up() { return Vector3(0.0, 1.0, 0.0); }
		static constexpr Vector3 Down() { return Vector3(0.0, -1.0, 0.0); }
		static constexpr Vector3 Right() { return Vector3(1.0, 0.0, 0.0); }
		static constexpr Vector3 Left() { return Vector3(-1.0, 0.0, 0.0); }
		static constexpr Vector3 Forward() { return Vector3(0.0, 0.0, -1.0); }
		static constexpr Vector3 Backward() { return Vector3(0.0, 0.0, 1.0); }

		constexpr Vector3() = default;

		constexpr Vector3(Vector2 value, double z) :
			X(value.X), Y(value.Y), Z(z) {}

		constexpr Vector3(double x, double y, double z) :
			X(x), Y(y), Z(z) {}

		constexpr Vector3(double value) :
			X(value), Y(value), Z(value) {}

		static constexpr double Distance(Vector3 const& value1, Vector3 const& value2) {
			const auto num1 = value1.X - value2.X;
			const auto num2 = value1.Y - value2.Y;
			const auto num3 = value1.Z - value2.Z;
			return Math::Sqrt(num1 * num1 + num2 * num2 + num3 * num3);
		}

		static constexpr double DistanceSquared(Vector3 const& value1, Vector3 const& value2) {
			const auto num1 = value1.X - value2.X;
			const auto num2 = value1.Y - value2.Y;
			const auto num3 = value1.Z - value2.Z;

			return num1 * num1 + num2 * num2 + num3 * num3;
		}

		static constexpr double Dot(Vector3 const& vector1, Vector3 const& vector2) {
			return vector1.X * vector2.X + vector1.Y * vector2.Y + vector1.Z * vector2.Z;
		}

		static constexpr Vector3 Normalize(Vector3 const& value) {
			const auto num = 1.0 / Math::Sqrt(value.X * value.X + value.Y * value.Y + value.Z * value.Z);
			return Vector3(
				value.X * num,
				value.Y * num,
				value.Z * num);
		}

		static constexpr Vector3 Cross(Vector3 const& vector1, Vector3 const& vector2) {
			Vector3 vector3;
			vector3.X = vector1.Y * vector2.Z - vector1.Z * vector2.Y;
			vector3.Y = vector1.Z * vector2.X - vector1.X * vector2.Z;
			vector3.Z = vector1.X * vector2.Y - vector1.Y * vector2.X;

			return vector3;
		}

		static constexpr Vector3 Reflect(Vector3 const& vector, Vector3 const& normal) {
			const auto num = vector.X * normal.X + vector.Y * normal.Y + vector.Z * normal.Z;

			Vector3 vector3;
			vector3.X = vector.X - 2.0 * num * normal.X;
			vector3.Y = vector.Y - 2.0 * num * normal.Y;
			vector3.Z = vector.Z - 2.0 * num * normal.Z;

			return vector3;
		}

		static constexpr Vector3 Min(Vector3 const& value1, Vector3 const& value2) {
			Vector3 vector3;
			vector3.X = value1.X < value2.X ? value1.X : value2.X;
			vector3.Y = value1.Y < value2.Y ? value1.Y : value2.Y;
			vector3.Z = value1.Z < value2.Z ? value1.Z : value2.Z;

			return vector3;
		}

		static constexpr Vector3 Max(Vector3 const& value1, Vector3 const& value2) {
			Vector3 vector3;
			vector3.X = value1.X > value2.X ? value1.X : value2.X;
			vector3.Y = value1.Y > value2.Y ? value1.Y : value2.Y;
			vector3.Z = value1.Z > value2.Z ? value1.Z : value2.Z;

			return vector3;
		}

		static constexpr Vector3 Clamp(Vector3 const& value1, Vector3 const& min, Vector3 const& max) {
			const auto x = value1.X;
			const auto num1 = x > max.X ? max.X : x;
			const auto num2 = num1 < min.X ? min.X : num1;
			const auto y = value1.Y;
			const auto num3 = y > max.Y ? max.Y : y;
			const auto num4 = num3 < min.Y ? min.Y : num3;
			const auto z = value1.Z;
			const auto num5 = z > max.Z ? max.Z : z;
			const auto num6 = num5 < min.Z ? min.Z : num5;

			Vector3 vector3;
			vector3.X = num2;
			vector3.Y = num4;
			vector3.Z = num6;

			return vector3;
		}

		static constexpr Vector3 Lerp(Vector3 const& value1, Vector3 const& value2, double amount) {
			Vector3 vector3;
			vector3.X = value1.X + (value2.X - value1.X) * amount;
			vector3.Y = value1.Y + (value2.Y - value1.Y) * amount;
			vector3.Z = value1.Z + (value2.Z - value1.Z) * amount;

			return vector3;
		}

		static constexpr Vector3 Barycentric(Vector3 const& value1, Vector3 const& value2,
			Vector3 const& value3, double amount1, double amount2) {
			Vector3 vector3;
			vector3.X = value1.X + amount1 * (value2.X - value1.X) + amount2 * (value3.X - value1.X);
			vector3.Y = value1.Y + amount1 * (value2.Y - value1.Y) + amount2 * (value3.Y - value1.Y);
			vector3.Z = value1.Z + amount1 * (value2.Z - value1.Z) + amount2 * (value3.Z - value1.Z);

			return vector3;
		}

		static constexpr Vector3 SmoothStep(Vector3 const& value1, Vector3 const& value2, double amount) {
			amount = amount > 1.0 ? 1.0 : amount < 0.0 ? 0.0 : amount;
			amount = amount * amount * 3.0 - 2.0 * amount;
			Vector3 vector3;
			vector3.X = value1.X + (value2.X - value1.X) * amount;
			vector3.Y = value1.Y + (value2.Y - value1.Y) * amount;
			vector3.Z = value1.Z + (value2.Z - value1.Z) * amount;
			return vector3;
		}

		static constexpr Vector3 CatmullRom(Vector3 const& value1, Vector3 const& value2,
			Vector3 const& value3, Vector3 const& value4, double amount) {
			const auto num1 = amount * amount;
			const auto num2 = amount * num1;

			Vector3 vector3;
			vector3.X = 0.5 * (2.0 * value2.X + (-value1.X + value3.X) * amount + (2.0 * value1.X - 5.0 * value2.X + 4.0 * value3.X - value4.X) * num1 + (-value1.X + 3.0 * value2.X - 3.0 * value3.X + value4.X) * num2);
			vector3.Y = 0.5 * (2.0 * value2.Y + (-value1.Y + value3.Y) * amount + (2.0 * value1.Y - 5.0 * value2.Y + 4.0 * value3.Y - value4.Y) * num1 + (-value1.Y + 3.0 * value2.Y - 3.0 * value3.Y + value4.Y) * num2);
			vector3.Z = 0.5 * (2.0 * value2.Z + (-value1.Z + value3.Z) * amount + (2.0 * value1.Z - 5.0 * value2.Z + 4.0 * value3.Z - value4.Z) * num1 + (-value1.Z + 3.0 * value2.Z - 3.0 * value3.Z + value4.Z) * num2);
			return vector3;
		}

		static constexpr Vector3 Hermite(Vector3 const& value1, Vector3 const& tangent1,
			Vector3 const& value2, Vector3 const& tangent2, double amount) {
			const auto num1 = amount * amount;
			const auto num2 = amount * num1;
			const auto num3 = (2.0 * num2 - 3.0 * num1 + 1.0);
			const auto num4 = (-2.0 * num2 + 3.0 * num1);
			const auto num5 = num2 - 2.0 * num1 + amount;
			const auto num6 = num2 - num1;

			Vector3 vector3;
			vector3.X = value1.X * num3 + value2.X * num4 + tangent1.X * num5 + tangent2.X * num6;
			vector3.Y = value1.Y * num3 + value2.Y * num4 + tangent1.Y * num5 + tangent2.Y * num6;
			vector3.Z = value1.Z * num3 + value2.Z * num4 + tangent1.Z * num5 + tangent2.Z * num6;
			return vector3;
		}

		static constexpr Vector3 Negate(Vector3 const& value) {
			Vector3 vector3;
			vector3.X = -value.X;
			vector3.Y = -value.Y;
			vector3.Z = -value.Z;
			return vector3;
		}

		static constexpr Vector3 Add(Vector3 const& value1, Vector3 const& value2) {
			Vector3 vector3;
			vector3.X = value1.X + value2.X;
			vector3.Y = value1.Y + value2.Y;
			vector3.Z = value1.Z + value2.Z;
			return vector3;
		}

		static constexpr Vector3 Subtract(Vector3 const& value1, Vector3 const& value2) {
			Vector3 vector3;
			vector3.X = value1.X - value2.X;
			vector3.Y = value1.Y - value2.Y;
			vector3.Z = value1.Z - value2.Z;
			return vector3;
		}

		static constexpr Vector3 Multiply(Vector3 const& value1, Vector3 const& value2) {
			Vector3 vector3;
			vector3.X = value1.X * value2.X;
			vector3.Y = value1.Y * value2.Y;
			vector3.Z = value1.Z * value2.Z;
			return vector3;
		}

		static constexpr Vector3 Multiply(Vector3 const& value1, double scaleFactor) {
			Vector3 vector3;
			vector3.X = value1.X * scaleFactor;
			vector3.Y = value1.Y * scaleFactor;
			vector3.Z = value1.Z * scaleFactor;
			return vector3;
		}

		static constexpr Vector3 Divide(Vector3 const& value1, Vector3 const& value2) {
			Vector3 vector3;
			vector3.X = value1.X / value2.X;
			vector3.Y = value1.Y / value2.Y;
			vector3.Z = value1.Z / value2.Z;
			return vector3;
		}

		static constexpr Vector3 Divide(Vector3 const& value1, double value2) {
			const auto num = 1.0 / value2;
			Vector3 vector3;
			vector3.X = value1.X * num;
			vector3.Y = value1.Y * num;
			vector3.Z = value1.Z * num;
			return vector3;
		}

		constexpr bool Equals(Vector3 const& other) const {
			return X == other.X && Y == other.Y && Z == other.Z;
		}

		constexpr double Length() const {
			return Math::Sqrt(X * X + Y * Y + Z * Z);
		}

		constexpr double LengthSquared() const {
			return X * X + Y * Y + Z * Z;
		}

		constexpr void Normalize() {
			const auto num = 1.0 / Math::Sqrt(X * X + Y * Y + Z * Z);
			X *= num;
			Y *= num;
			Z *= num;
		}

		constexpr Vector3 operator-() {
			return Negate(*this);
		}

		friend constexpr bool operator==(Vector3 const& value1, Vector3 const& value2) {
			return value1.Equals(value2);
		}

		friend constexpr bool operator!=(Vector3 const& value1, Vector3 const& value2) {
			return !value1.Equals(value2);
		}

		friend constexpr Vector3 operator+(Vector3 const& value1, Vector3 const& value2) {
			return Vector3::Add(value1, value2);
		}

		friend constexpr Vector3 operator-(Vector3 const& value1, Vector3 const& value2) {
			return Vector3::Subtract(value1, value2);
		}

		friend constexpr Vector3 operator*(Vector3 const& value1, Vector3 const& value2) {
			return Vector3::Multiply(value1, value2);
		}

		friend constexpr Vector3 operator*(Vector3 const& value, double scaleFactor) {
			return Vector3::Multiply(value, scaleFactor);
		}

		friend constexpr Vector3 operator*(double scaleFactor, Vector3 const& value) {
			return Vector3::Multiply(value, scaleFactor);
		}

		friend constexpr Vector3 operator/(Vector3 const& value1, Vector3 const& value2) {
			return Vector3::Divide(value1, value2);
		}

		friend constexpr Vector3 operator/(Vector3 const& value, double divider) {
			return Vector3::Divide(value, divider);
		}

		//Implementado em matrix.hpp

		static constexpr Vector3 Transform(Vector3 const& position, Matrix const& matrix);
		static constexpr Vector3 TransformNormal(Vector3 const& normal, Matrix const& matrix);
		static void Transform(std::vector<Vector3> const& sourceArray, Matrix const& matrix, std::vector<Vector3>& destinationArray);
		static void TransformNormal(std::vector<Vector3> const& sourceArray, Matrix const& matrix, std::vector<Vector3>& destinationArray);
		static void Transform(std::vector<Vector3> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector3>& destinationArray, size_t destinationIndex, size_t length);
		static void TransformNormal(std::vector<Vector3> const& sourceArray, size_t sourceIndex, Matrix& matrix, std::vector<Vector3> destinationArray, size_t destinationIndex, size_t length);

		//Implementado em quaternion.hpp

		static constexpr Vector3 Transform(Vector3 const& value, Quaternion const& rotation);
		static void Transform(std::vector<Vector3> const& sourceArray, Quaternion const& rotation, std::vector<Vector3>& destinationArray);
		static void Transform(std::vector<Vector3> const& sourceArray, size_t sourceIndex, Quaternion const& rotation, std::vector<Vector3>& destinationArray, size_t destinationIndex, size_t length);
	};

	using Vec3 = Vector3;
}

//Vector4
namespace xna {
	struct Matrix;
	struct Quaternion;

	struct Vector4 {
		double X{ 0.0 };
		double Y{ 0.0 };
		double Z{ 0.0 };
		double W{ 0.0 };

		static constexpr Vector4 Zero() { return Vector4(); }
		static constexpr Vector4 One() { return Vector4(1.0); }
		static constexpr Vector4 UnitX() { return Vector4(1.0, 0.0, 0.0, 0.0); }
		static constexpr Vector4 UnitY() { return Vector4(0.0, 1.0, 0.0, 0.0); }
		static constexpr Vector4 UnitZ() { return Vector4(0.0, 0.0, 1.0, 0.0); }
		static constexpr Vector4 UnitW() { return Vector4(0.0, 0.0, 0.0, 1.0); }

		constexpr Vector4() = default;

		constexpr Vector4(Vector2 value, double z, double w) :
			X(value.X), Y(value.Y), Z(z), W(w) {}

		constexpr Vector4(Vector3 value, double w) :
			X(value.X), Y(value.Y), Z(value.Z), W(w) {}

		constexpr Vector4(double x, double y, double z, double w) :
			X(x), Y(y), Z(z), W(w) {}

		constexpr Vector4(double value) :
			X(value), Y(value), Z(value), W(value) {}

		constexpr bool Equals(Vector4 const& other) const {
			return X == other.X
				&& Y == other.Y
				&& Z == other.Z
				&& W == other.W;
		}

		static constexpr double Distance(Vector4 const& value1, Vector4 const& value2) {
			const auto num1 = value1.X - value2.X;
			const auto num2 = value1.Y - value2.Y;
			const auto num3 = value1.Z - value2.Z;
			const auto num4 = value1.W - value2.W;
			return Math::Sqrt(num1 * num1 + num2 * num2 + num3 * num3 + num4 * num4);
		}

		static constexpr double DistanceSquared(Vector4 const& value1, Vector4 const& value2) {
			const auto num1 = value1.X - value2.X;
			const auto num2 = value1.Y - value2.Y;
			const auto num3 = value1.Z - value2.Z;
			const auto num4 = value1.W - value2.W;
			return (num1 * num1 + num2 * num2 + num3 * num3 + num4 * num4);
		}

		static constexpr double Dot(Vector4 const& vector1, Vector4 const& vector2) {
			return vector1.X * vector2.X + vector1.Y * vector2.Y + vector1.Z * vector2.Z + vector1.W * vector2.W;
		}

		static constexpr Vector4 Min(Vector4 const& value1, Vector4 const& value2) {
			return Vector4(
				value1.X < value2.X ? value1.X : value2.X,
				value1.Y < value2.Y ? value1.Y : value2.Y,
				value1.Z < value2.Z ? value1.Z : value2.Z,
				value1.W < value2.W ? value1.W : value2.W);
		}

		static constexpr Vector4 Max(Vector4 const& value1, Vector4 const& value2) {
			return Vector4(
				value1.X > value2.X ? value1.X : value2.X,
				value1.Y > value2.Y ? value1.Y : value2.Y,
				value1.Z > value2.Z ? value1.Z : value2.Z,
				value1.W > value2.W ? value1.W : value2.W);
		}

		static constexpr Vector4 Clamp(Vector4 const& value1, Vector4 const& min, Vector4 const& max) {
			const auto x = value1.X;
			const auto num1 = x > max.X ? max.X : x;
			const auto num2 = num1 < min.X ? min.X : num1;
			const auto y = value1.Y;
			const auto num3 = y > max.Y ? max.Y : y;
			const auto num4 = num3 < min.Y ? min.Y : num3;
			const auto z = value1.Z;
			const auto num5 = z > max.Z ? max.Z : z;
			const auto num6 = num5 < min.Z ? min.Z : num5;
			const auto w = value1.W;
			const auto num7 = w > max.W ? max.W : w;
			const auto num8 = num7 < min.W ? min.W : num7;

			return Vector4(num2, num4, num6, num8);
		}

		static constexpr Vector4 Lerp(Vector4 const& value1, Vector4 const& value2, double amount) {
			Vector4 vector4;
			vector4.X = value1.X + (value2.X - value1.X) * amount;
			vector4.Y = value1.Y + (value2.Y - value1.Y) * amount;
			vector4.Z = value1.Z + (value2.Z - value1.Z) * amount;
			vector4.W = value1.W + (value2.W - value1.W) * amount;
			return vector4;
		}

		static constexpr Vector4 Barycentric(Vector4 const& value1, Vector4 const& value2, Vector4 const& value3, double amount1, double amount2) {
			Vector4 vector4;
			vector4.X = (value1.X + amount1 * (value2.X - value1.X) + amount2 * (value3.X - value1.X));
			vector4.Y = (value1.Y + amount1 * (value2.Y - value1.Y) + amount2 * (value3.Y - value1.Y));
			vector4.Z = (value1.Z + amount1 * (value2.Z - value1.Z) + amount2 * (value3.Z - value1.Z));
			vector4.W = (value1.W + amount1 * (value2.W - value1.W) + amount2 * (value3.W - value1.W));
			return vector4;
		}

		static constexpr Vector4 SmoothStep(Vector4 const& value1, Vector4 const& value2, double amount) {
			amount = amount > 1.0 ? 1.0 : (amount < 0.0 ? 0.0 : amount);
			amount = (amount * amount * (3.0 - 2.0 * amount));

			Vector4 vector4;
			vector4.X = value1.X + (value2.X - value1.X) * amount;
			vector4.Y = value1.Y + (value2.Y - value1.Y) * amount;
			vector4.Z = value1.Z + (value2.Z - value1.Z) * amount;
			vector4.W = value1.W + (value2.W - value1.W) * amount;
			return vector4;
		}

		static constexpr Vector4 CatmullRom(Vector4 const& value1, Vector4 const& value2, Vector4 const& value3, Vector4 const& value4, double amount) {
			const auto num1 = amount * amount;
			const auto num2 = amount * num1;

			Vector4 vector4;
			vector4.X = (0.5 * (2.0 * value2.X + (-value1.X + value3.X) * amount + (2.0 * value1.X - 5.0 * value2.X + 4.0 * value3.X - value4.X) * num1 + (-value1.X + 3.0 * value2.X - 3.0 * value3.X + value4.X) * num2));
			vector4.Y = (0.5 * (2.0 * value2.Y + (-value1.Y + value3.Y) * amount + (2.0 * value1.Y - 5.0 * value2.Y + 4.0 * value3.Y - value4.Y) * num1 + (-value1.Y + 3.0 * value2.Y - 3.0 * value3.Y + value4.Y) * num2));
			vector4.Z = (0.5 * (2.0 * value2.Z + (-value1.Z + value3.Z) * amount + (2.0 * value1.Z - 5.0 * value2.Z + 4.0 * value3.Z - value4.Z) * num1 + (-value1.Z + 3.0 * value2.Z - 3.0 * value3.Z + value4.Z) * num2));
			vector4.W = (0.5 * (2.0 * value2.W + (-value1.W + value3.W) * amount + (2.0 * value1.W - 5.0 * value2.W + 4.0 * value3.W - value4.W) * num1 + (-value1.W + 3.0 * value2.W - 3.0 * value3.W + value4.W) * num2));
			return vector4;
		}

		static constexpr Vector4 Hermite(Vector4 const& value1, Vector4 const& tangent1, Vector4 const& value2, Vector4 const& tangent2, double amount) {
			const auto num1 = amount * amount;
			const auto num2 = amount * num1;
			const auto num3 = (2.0 * num2 - 3.0 * num1 + 1.0);
			const auto num4 = (-2.0 * num2 + 3.0 * num1);
			const auto num5 = num2 - 2.0 * num1 + amount;
			const auto num6 = num2 - num1;

			Vector4 vector4;
			vector4.X = (value1.X * num3 + value2.X * num4 + tangent1.X * num5 + tangent2.X * num6);
			vector4.Y = (value1.Y * num3 + value2.Y * num4 + tangent1.Y * num5 + tangent2.Y * num6);
			vector4.Z = (value1.Z * num3 + value2.Z * num4 + tangent1.Z * num5 + tangent2.Z * num6);
			vector4.W = (value1.W * num3 + value2.W * num4 + tangent1.W * num5 + tangent2.W * num6);
			return vector4;
		}

		static constexpr Vector4 Normalize(Vector4 const& vector) {
			const auto num = 1.0 / Math::Sqrt(vector.X * vector.X + vector.Y * vector.Y + vector.Z * vector.Z + vector.W * vector.W);
			Vector4 vector4;
			vector4.X = vector.X * num;
			vector4.Y = vector.Y * num;
			vector4.Z = vector.Z * num;
			vector4.W = vector.W * num;
			return vector4;
		}

		static constexpr Vector4 Negate(Vector4 const& value) {
			Vector4 vector4;
			vector4.X = -value.X;
			vector4.Y = -value.Y;
			vector4.Z = -value.Z;
			vector4.W = -value.W;
			return vector4;
		}

		static constexpr Vector4 Add(Vector4 const& value1, Vector4 const& value2) {
			Vector4 vector4;
			vector4.X = value1.X + value2.X;
			vector4.Y = value1.Y + value2.Y;
			vector4.Z = value1.Z + value2.Z;
			vector4.W = value1.W + value2.W;
			return vector4;
		}

		static constexpr Vector4 Subtract(Vector4 const& value1, Vector4 const& value2) {
			Vector4 vector4;
			vector4.X = value1.X - value2.X;
			vector4.Y = value1.Y - value2.Y;
			vector4.Z = value1.Z - value2.Z;
			vector4.W = value1.W - value2.W;
			return vector4;
		}

		static constexpr Vector4 Multiply(Vector4 const& value1, Vector4 const& value2) {
			Vector4 vector4;
			vector4.X = value1.X * value2.X;
			vector4.Y = value1.Y * value2.Y;
			vector4.Z = value1.Z * value2.Z;
			vector4.W = value1.W * value2.W;
			return vector4;
		}

		static constexpr Vector4 Multiply(Vector4 const& value1, double scaleFactor) {
			Vector4 vector4;
			vector4.X = value1.X * scaleFactor;
			vector4.Y = value1.Y * scaleFactor;
			vector4.Z = value1.Z * scaleFactor;
			vector4.W = value1.W * scaleFactor;
			return vector4;
		}

		static constexpr Vector4 Divide(Vector4 const& value1, Vector4 const& value2) {
			Vector4 vector4;
			vector4.X = value1.X / value2.X;
			vector4.Y = value1.Y / value2.Y;
			vector4.Z = value1.Z / value2.Z;
			vector4.W = value1.W / value2.W;
			return vector4;
		}

		static constexpr Vector4 Divide(Vector4 const& value1, double divider) {
			const auto num = 1.0 / divider;
			Vector4 vector4;
			vector4.X = value1.X * num;
			vector4.Y = value1.Y * num;
			vector4.Z = value1.Z * num;
			vector4.W = value1.W * num;
			return vector4;
		}

		constexpr double Length() const {
			return Math::Sqrt(X * X + Y * Y + Z * Z + W * W);
		}

		constexpr double LengthSquared() const {
			return X * X + Y * Y + Z * Z + W * W;
		}

		constexpr void Normalize() {
			const auto num = 1.0 / Math::Sqrt(X * X + Y * Y + Z * Z + W * W);
			X *= num;
			Y *= num;
			Z *= num;
			W *= num;
		}

		constexpr Vector4 operator-() {
			return Negate(*this);
		}

		friend constexpr bool operator==(Vector4 const& value1, Vector4 const& value2) {
			return value1.Equals(value2);
		}

		friend constexpr bool operator!=(Vector4 const& value1, Vector4 const& value2) {
			return !value1.Equals(value2);
		}

		friend constexpr Vector4 operator+(Vector4 const& value1, Vector4 const& value2) {
			return Vector4::Add(value1, value2);
		}

		friend constexpr Vector4 operator-(Vector4 const& value1, Vector4 const& value2) {
			return Vector4::Subtract(value1, value2);
		}

		friend constexpr Vector4 operator*(Vector4 const& value1, Vector4 const& value2) {
			return Vector4::Multiply(value1, value2);
		}

		friend constexpr Vector4 operator*(Vector4 const& value, double scaleFactor) {
			return Vector4::Multiply(value, scaleFactor);
		}

		friend constexpr Vector4 operator*(double scaleFactor, Vector4 const& value) {
			return Vector4::Multiply(value, scaleFactor);
		}

		friend constexpr Vector4 operator/(Vector4 const& value1, Vector4 const& value2) {
			return Vector4::Divide(value1, value2);
		}

		friend constexpr Vector4 operator/(Vector4 const& value, double divider) {
			return Vector4::Divide(value, divider);
		}

		//Implementado em matrix.hpp

		static constexpr Vector4 Transform(Vector2 const& position, Matrix const& matrix);
		static constexpr Vector4 Transform(Vector3 const& position, Matrix const& matrix);
		static constexpr Vector4 Transform(Vector4 const& vector, Matrix const& matrix);
		static void Transform(std::vector<Vector4> const& sourceArray, Matrix const& matrix, std::vector<Vector4>& destinationArray);
		static void Transform(std::vector<Vector4> const& sourceArray, size_t sourceIndex, Matrix const& matrix, std::vector<Vector4>& destinationArray, size_t destinationIndex, size_t length);

		//Implementado em quaternion.hpp

		static constexpr Vector4 Transform(Vector2 const& value, Quaternion const& rotation);
		static constexpr Vector4 Transform(Vector3 const& value, Quaternion const& rotation);
		static constexpr Vector4 Transform(Vector4 const& value, Quaternion const& rotation);
		static void Transform(std::vector<Vector4> const& sourceArray, Quaternion const& rotation, std::vector<Vector4>& destinationArray);
		static void Transform(std::vector<Vector4> const& sourceArray, size_t sourceIndex, Quaternion const& rotation, std::vector<Vector4>& destinationArray, size_t destinationIndex, size_t length);
	};

	using Vec4 = Vector4;
}

#endif