#ifndef XNA_STRUCTS_HPP
#define XNA_STRUCTS_HPP

#include <vector>
#include <cmath>
#include "mathhelper.hpp"
#include "csharp/IntegralNumeric.hpp"
#include "csharp/nullable.hpp"

//Point
namespace xna {
	struct Point {
		csint X{ 0 };
		csint Y{ 0 };

		constexpr Point() = default;
		constexpr Point(csint x, csint y) :
			X(x), Y(y) {}

		static constexpr Point Zero() { return Point(); }

		bool constexpr Equals(Point const& other) const {
			return X == other.X && Y == other.Y;
		}

		constexpr Point operator-() {
			return Point(-X, -Y);
		}

		friend constexpr bool operator==(Point const& a, Point const& b) {
			return a.Equals(b);
		}

		friend constexpr bool operator!=(Point const& a, Point const& b) {
			return !a.Equals(b);
		}
	};
}

//Rectangle
namespace xna {
	struct Rectangle {
		csint X{ 0 };
		csint Y{ 0 };
		csint Width{ 0 };
		csint Height{ 0 };

		constexpr Rectangle() = default;

		constexpr Rectangle(csint x, csint y, csint width, csint height) :
			X(x), Y(y), Width(width), Height(height) {}

		static constexpr Rectangle Empty() {
			return Rectangle(0, 0, 0, 0);
		}

		constexpr csint Left() const {
			return X;
		}

		constexpr csint Right() const {
			return X + Width;
		}

		constexpr csint Top() const {
			return Y;
		}

		constexpr csint Bottom() const {
			return Y + Height;
		}

		constexpr void Offset(Point const& amount) {
			X += amount.X;
			Y += amount.Y;
		}

		constexpr void Offset(csint offsetX, csint offsetY) {
			X += offsetX;
			Y += offsetY;
		}

		constexpr void Inflate(csint horizontalAmount, csint verticalAmount) {
			X -= horizontalAmount;
			Y -= verticalAmount;
			Width += horizontalAmount * 2;
			Height += verticalAmount * 2;
		}

		constexpr bool Contains(csint x, csint y) const {
			return X <= x && x < X + Width && Y <= y && y < Y + Height;
		}

		constexpr bool Contains(Point const& value) const {
			return X <= value.X && value.X < X + Width && Y <= value.Y && value.Y < Y + Height;
		}

		constexpr bool Contains(Rectangle const& value) const {
			return X <= value.X && value.X + value.Width <= X + Width && Y <= value.Y && value.Y + value.Height <= Y + Height;
		}

		constexpr bool Intersects(Rectangle const& value) const {
			return value.X < X + Width && X < value.X + value.Width && value.Y < Y + Height && Y < value.Y + value.Height;
		}

		constexpr bool Equals(Rectangle const& other) const {
			return X == other.X && Y == other.Y && Width == other.Width && Height == other.Height;
		}

		static constexpr Rectangle Intersect(Rectangle const& value1, Rectangle const& value2) {
			csint num1 = value1.X + value1.Width;
			csint num2 = value2.X + value2.Width;
			csint num3 = value1.Y + value1.Height;
			csint num4 = value2.Y + value2.Height;
			csint num5 = value1.X > value2.X ? value1.X : value2.X;
			csint num6 = value1.Y > value2.Y ? value1.Y : value2.Y;
			csint num7 = num1 < num2 ? num1 : num2;
			csint num8 = num3 < num4 ? num3 : num4;
			Rectangle rectangle;

			if (num7 > num5 && num8 > num6) {
				rectangle.X = num5;
				rectangle.Y = num6;
				rectangle.Width = num7 - num5;
				rectangle.Height = num8 - num6;
			}
			else {
				rectangle.X = 0;
				rectangle.Y = 0;
				rectangle.Width = 0;
				rectangle.Height = 0;
			}

			return rectangle;
		}

		static constexpr Rectangle Union(Rectangle const& value1, Rectangle const& value2) {
			csint num1 = value1.X + value1.Width;
			csint num2 = value2.X + value2.Width;
			csint num3 = value1.Y + value1.Height;
			csint num4 = value2.Y + value2.Height;
			csint num5 = value1.X < value2.X ? value1.X : value2.X;
			csint num6 = value1.Y < value2.Y ? value1.Y : value2.Y;
			csint num7 = num1 > num2 ? num1 : num2;
			csint num8 = num3 > num4 ? num3 : num4;

			Rectangle rectangle;
			rectangle.X = num5;
			rectangle.Y = num6;
			rectangle.Width = num7 - num5;
			rectangle.Height = num8 - num6;

			return rectangle;
		}

		friend constexpr bool operator==(Rectangle const& value1, Rectangle const& value2) {
			return value1.Equals(value2);
		}

		friend constexpr bool operator!=(Rectangle const& value1, Rectangle const& value2) {
			return !value1.Equals(value2);
		}
	};
}

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

//Circle
namespace xna {
	struct Circle {
		double Radius{ 0 };
		double X{ 0 };
		double Y{ 0 };		

		Circle(double x, double y, double radius) :
			X(x), Y(y), Radius(radius) {
		}

		Circle(Vector2 center, double radius):
			X(center.X), Y(center.Y), Radius(radius) {
		}

		constexpr Vector2 Center() const {
			return Vector2(X, Y);
		}

		constexpr bool Intersects(Circle const& other) const {
			return Vector2::Distance(Center(), other.Center()) < Radius + other.Radius;
		}

		constexpr bool Intersects(Rectangle const& rectangle) const {
			const auto center = Center();
			const Vector2 v(
				MathHelper::Clamp(center.X, rectangle.Left(), rectangle.Right()),
				MathHelper::Clamp(center.Y, rectangle.Top(), rectangle.Bottom()));

			const auto direction = center - v;
			const auto distanceSquared = direction.LengthSquared();

			return ((distanceSquared > 0) && (distanceSquared < Radius * Radius));
		}

		constexpr bool Equals(Circle const& other) const {
			return Radius == other.Radius
				&& X == other.X
				&& Y == other.Y;
		}

		friend constexpr bool operator== (Circle const& a, Circle const& b) {
			return a.Equals(b);
		}
		
		friend constexpr bool operator!= (Circle const& a, Circle const& b) {
			return !a.Equals(b);
		}
	};
}

//Matrix
namespace xna {
	struct Plane;
	struct Quaternion;

	struct Matrix {
		double M11{ 0.0 };
		double M12{ 0.0 };
		double M13{ 0.0 };
		double M14{ 0.0 };
		double M21{ 0.0 };
		double M22{ 0.0 };
		double M23{ 0.0 };
		double M24{ 0.0 };
		double M31{ 0.0 };
		double M32{ 0.0 };
		double M33{ 0.0 };
		double M34{ 0.0 };
		double M41{ 0.0 };
		double M42{ 0.0 };
		double M43{ 0.0 };
		double M44{ 0.0 };

		constexpr Matrix() = default;

		constexpr Matrix(double m11, double m12, double m13, double m14,
			double m21, double m22, double m23, double m24,
			double m31, double m32, double m33, double m34,
			double m41, double m42, double m43, double m44) :
			M11(m11), M12(m12), M13(m13), M14(m14),
			M21(m21), M22(m22), M23(m23), M24(m24),
			M31(m31), M32(m32), M33(m33), M34(m34),
			M41(m41), M42(m42), M43(m43), M44(m44) {}

		constexpr Matrix(Vector4 row1, Vector4 row2, Vector4 row3, Vector4 row4) :
			M11(row1.X), M12(row1.Y), M13(row1.Z), M14(row1.W),
			M21(row2.X), M22(row2.Y), M23(row2.Z), M24(row2.W),
			M31(row3.X), M32(row3.Y), M33(row3.Z), M34(row3.W),
			M41(row4.X), M42(row4.Y), M43(row4.Z), M44(row4.W) {}

		constexpr Matrix Identity() const {
			return Matrix(
				1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0);
		}

		constexpr Vector3 Up() const {
			Vector3 up;
			up.X = M21;
			up.Y = M22;
			up.Z = M23;
			return up;
		}

		constexpr void Up(Vector3 const& value) {
			M21 = value.X;
			M22 = value.Y;
			M23 = value.Z;
		}

		constexpr Vector3 Down() const {
			Vector3 down;
			down.X = -M21;
			down.Y = -M22;
			down.Z = -M23;
			return down;
		}

		constexpr void Down(Vector3 const& value) {
			M21 = -value.X;
			M22 = -value.Y;
			M23 = -value.Z;
		}

		constexpr Vector3 Right() const {
			Vector3 right;
			right.X = M11;
			right.Y = M12;
			right.Z = M13;
			return right;
		}

		constexpr void Right(Vector3 const& value) {
			M11 = value.X;
			M12 = value.Y;
			M13 = value.Z;
		}

		constexpr Vector3 Left() const {
			Vector3 left;
			left.X = -M11;
			left.Y = -M12;
			left.Z = -M13;
			return left;
		}

		constexpr void Left(Vector3 const& value) {
			M11 = -value.X;
			M12 = -value.Y;
			M13 = -value.Z;
		}

		constexpr Vector3 Forward() const {
			Vector3 forward;
			forward.X = -M31;
			forward.Y = -M32;
			forward.Z = -M33;
			return forward;
		}

		constexpr void Forward(Vector3 const& value) {
			M31 = -value.X;
			M32 = -value.Y;
			M33 = -value.Z;
		}

		constexpr Vector3 Backward() const {
			Vector3 backward;
			backward.X = M31;
			backward.Y = M32;
			backward.Z = M33;
			return backward;
		}

		constexpr void Backward(Vector3 const& value) {
			M31 = value.X;
			M32 = value.Y;
			M33 = value.Z;
		}

		constexpr Vector3 Translation() const {
			Vector3 translation;
			translation.X = M41;
			translation.Y = M42;
			translation.Z = M43;
			return translation;
		}

		constexpr void Translation(Vector3 const& value) {
			M41 = value.X;
			M42 = value.Y;
			M43 = value.Z;
		}

		static Matrix CreateBillboard(Vector3 const& objectPosition, Vector3 const& cameraPosition, Vector3 cameraUpVector, cs::Nullable<Vector3> const& cameraForwardVector);
		static Matrix CreateConstrainedBillboard(Vector3 const& objectPosition, Vector3 const& cameraPosition, Vector3 const& rotateAxis,
			cs::Nullable<Vector3> const& cameraForwardVector, cs::Nullable<Vector3> const& objectForwardVector);

		static constexpr Matrix CreateTranslation(Vector3 const& position) {
			Matrix translation;
			translation.M11 = 1.0f;
			translation.M12 = 0.0f;
			translation.M13 = 0.0f;
			translation.M14 = 0.0f;
			translation.M21 = 0.0f;
			translation.M22 = 1.0f;
			translation.M23 = 0.0f;
			translation.M24 = 0.0f;
			translation.M31 = 0.0f;
			translation.M32 = 0.0f;
			translation.M33 = 1.0f;
			translation.M34 = 0.0f;
			translation.M41 = position.X;
			translation.M42 = position.Y;
			translation.M43 = position.Z;
			translation.M44 = 1.0f;
			return translation;
		}

		static constexpr Matrix CreateTranslation(double xPosition, double yPosition, double zPosition) {
			Matrix translation;
			translation.M11 = 1.0;
			translation.M12 = 0.0f;
			translation.M13 = 0.0f;
			translation.M14 = 0.0f;
			translation.M21 = 0.0f;
			translation.M22 = 1.0;
			translation.M23 = 0.0f;
			translation.M24 = 0.0f;
			translation.M31 = 0.0f;
			translation.M32 = 0.0f;
			translation.M33 = 1.0;
			translation.M34 = 0.0f;
			translation.M41 = xPosition;
			translation.M42 = yPosition;
			translation.M43 = zPosition;
			translation.M44 = 1.0;
			return translation;
		}

		static constexpr Matrix CreateScale(double xScale, double yScale, double zScale) {
			const auto num1 = xScale;
			const auto num2 = yScale;
			const auto num3 = zScale;

			Matrix scale;
			scale.M11 = num1;
			scale.M12 = 0.0;
			scale.M13 = 0.0;
			scale.M14 = 0.0;
			scale.M21 = 0.0;
			scale.M22 = num2;
			scale.M23 = 0.0;
			scale.M24 = 0.0;
			scale.M31 = 0.0;
			scale.M32 = 0.0;
			scale.M33 = num3;
			scale.M34 = 0.0;
			scale.M41 = 0.0;
			scale.M42 = 0.0;
			scale.M43 = 0.0;
			scale.M44 = 1.0;
			return scale;
		}

		static Matrix CreateScale(Vector3 const& scales) {
			const auto x = scales.X;
			const auto y = scales.Y;
			const auto z = scales.Z;

			Matrix scale;
			scale.M11 = x;
			scale.M12 = 0.0;
			scale.M13 = 0.0;
			scale.M14 = 0.0;
			scale.M21 = 0.0;
			scale.M22 = y;
			scale.M23 = 0.0;
			scale.M24 = 0.0;
			scale.M31 = 0.0;
			scale.M32 = 0.0;
			scale.M33 = z;
			scale.M34 = 0.0;
			scale.M41 = 0.0;
			scale.M42 = 0.0;
			scale.M43 = 0.0;
			scale.M44 = 1.0;
			return scale;
		}

		static constexpr Matrix CreateScale(double scale) {
			const auto num = scale;

			Matrix scale1;
			scale1.M11 = num;
			scale1.M12 = 0.0;
			scale1.M13 = 0.0;
			scale1.M14 = 0.0;
			scale1.M21 = 0.0;
			scale1.M22 = num;
			scale1.M23 = 0.0;
			scale1.M24 = 0.0;
			scale1.M31 = 0.0;
			scale1.M32 = 0.0;
			scale1.M33 = num;
			scale1.M34 = 0.0;
			scale1.M41 = 0.0;
			scale1.M42 = 0.0;
			scale1.M43 = 0.0;
			scale1.M44 = 1.0;
			return scale1;
		}

		static Matrix CreateRotationX(double radians);
		static Matrix CreateRotationY(double radians);
		static Matrix CreateRotationZ(double radians);
		static Matrix CreateFromAxisAngle(Vector3 const& axis, double angle);

		static Matrix CreatePerspectiveFieldOfView(double fieldOfView, double aspectRatio, double nearPlaneDistance, double farPlaneDistance) {
			if (fieldOfView <= 0.0 || fieldOfView >= 3.1415927410125732)
			{
				if (fieldOfView <= 0.0)
					fieldOfView = 0.5;
				else
					fieldOfView = 3.0;
			}

			if (nearPlaneDistance <= 0.0)
				nearPlaneDistance = 0.1;

			if (farPlaneDistance <= 0.0)
				farPlaneDistance = 1.0;

			if (nearPlaneDistance >= farPlaneDistance)
				farPlaneDistance = farPlaneDistance - 1.0;

			const auto num1 = 1.0 / tan(fieldOfView * 0.5);
			const auto num2 = num1 / aspectRatio;

			Matrix perspectiveFieldOfView;
			perspectiveFieldOfView.M11 = num2;
			perspectiveFieldOfView.M12 = perspectiveFieldOfView.M13 = perspectiveFieldOfView.M14 = 0.0;
			perspectiveFieldOfView.M22 = num1;
			perspectiveFieldOfView.M21 = perspectiveFieldOfView.M23 = perspectiveFieldOfView.M24 = 0.0;
			perspectiveFieldOfView.M31 = perspectiveFieldOfView.M32 = 0.0;
			perspectiveFieldOfView.M33 = farPlaneDistance / (nearPlaneDistance - farPlaneDistance);
			perspectiveFieldOfView.M34 = -1.0;
			perspectiveFieldOfView.M41 = perspectiveFieldOfView.M42 = perspectiveFieldOfView.M44 = 0.0;
			perspectiveFieldOfView.M43 = (nearPlaneDistance * farPlaneDistance / (nearPlaneDistance - farPlaneDistance));

			return perspectiveFieldOfView;
		}

		static constexpr Matrix CreatePerspective(double width, double height, double nearPlaneDistance, double farPlaneDistance) {
			if (nearPlaneDistance <= 0.0)
				nearPlaneDistance = 0.1;

			if (farPlaneDistance <= 0.0)
				farPlaneDistance = 1.0;

			if (nearPlaneDistance >= farPlaneDistance)
				farPlaneDistance = farPlaneDistance - 1.0;

			Matrix perspective;
			perspective.M11 = 2.0 * nearPlaneDistance / width;
			perspective.M12 = perspective.M13 = perspective.M14 = 0.0;
			perspective.M22 = 2.0 * nearPlaneDistance / height;
			perspective.M21 = perspective.M23 = perspective.M24 = 0.0;
			perspective.M33 = farPlaneDistance / (nearPlaneDistance - farPlaneDistance);
			perspective.M31 = perspective.M32 = 0.0;
			perspective.M34 = -1.0;
			perspective.M41 = perspective.M42 = perspective.M44 = 0.0;
			perspective.M43 = (nearPlaneDistance * farPlaneDistance / (nearPlaneDistance - farPlaneDistance));
			return perspective;
		}

		static constexpr Matrix CreatePerspectiveOffCenter(double left, double right, double bottom, double top, double nearPlaneDistance, double farPlaneDistance)
		{
			if (nearPlaneDistance <= 0.0)
				nearPlaneDistance = 0.1;

			if (farPlaneDistance <= 0.0)
				farPlaneDistance = 1.0;

			if (nearPlaneDistance >= farPlaneDistance)
				farPlaneDistance = farPlaneDistance - 1.0;

			Matrix perspectiveOffCenter;
			perspectiveOffCenter.M11 = (2.0 * nearPlaneDistance / (right - left));
			perspectiveOffCenter.M12 = perspectiveOffCenter.M13 = perspectiveOffCenter.M14 = 0.0f;
			perspectiveOffCenter.M22 = (2.0 * nearPlaneDistance / (top - bottom));
			perspectiveOffCenter.M21 = perspectiveOffCenter.M23 = perspectiveOffCenter.M24 = 0.0f;
			perspectiveOffCenter.M31 = ((left + right) / (right - left));
			perspectiveOffCenter.M32 = ((top + bottom) / (top - bottom));
			perspectiveOffCenter.M33 = farPlaneDistance / (nearPlaneDistance - farPlaneDistance);
			perspectiveOffCenter.M34 = -1.0;
			perspectiveOffCenter.M43 = (nearPlaneDistance * farPlaneDistance / (nearPlaneDistance - farPlaneDistance));
			perspectiveOffCenter.M41 = perspectiveOffCenter.M42 = perspectiveOffCenter.M44 = 0.0f;
			return perspectiveOffCenter;
		}

		static constexpr Matrix CreateOrthographic(double width, double height, double zNearPlane, double zFarPlane) {
			Matrix orthographic;
			orthographic.M11 = 2.0 / width;
			orthographic.M12 = orthographic.M13 = orthographic.M14 = 0.0f;
			orthographic.M22 = 2.0 / height;
			orthographic.M21 = orthographic.M23 = orthographic.M24 = 0.0f;
			orthographic.M33 = (1.0 / (zNearPlane - zFarPlane));
			orthographic.M31 = orthographic.M32 = orthographic.M34 = 0.0f;
			orthographic.M41 = orthographic.M42 = 0.0f;
			orthographic.M43 = zNearPlane / (zNearPlane - zFarPlane);
			orthographic.M44 = 1.0;
			return orthographic;
		}

		static constexpr Matrix CreateOrthographicOffCenter(double left, double right, double bottom, double top, double zNearPlane, double zFarPlane) {
			Matrix orthographicOffCenter;
			orthographicOffCenter.M11 = (2.0 / (right - left));
			orthographicOffCenter.M12 = orthographicOffCenter.M13 = orthographicOffCenter.M14 = 0.0f;
			orthographicOffCenter.M22 = (2.0 / (top - bottom));
			orthographicOffCenter.M21 = orthographicOffCenter.M23 = orthographicOffCenter.M24 = 0.0f;
			orthographicOffCenter.M33 = (1.0 / (zNearPlane - zFarPlane));
			orthographicOffCenter.M31 = orthographicOffCenter.M32 = orthographicOffCenter.M34 = 0.0f;
			orthographicOffCenter.M41 = ((left + right) / (left - right));
			orthographicOffCenter.M42 = ((top + bottom) / (bottom - top));
			orthographicOffCenter.M43 = zNearPlane / (zNearPlane - zFarPlane);
			orthographicOffCenter.M44 = 1.0;
			return orthographicOffCenter;
		}

		static Matrix CreateLookAt(Vector3 cameraPosition, Vector3 cameraTarget, Vector3 cameraUpVector) {
			Vector3 vector3_1 = Vector3::Normalize(cameraPosition - cameraTarget);
			Vector3 vector3_2 = Vector3::Normalize(Vector3::Cross(cameraUpVector, vector3_1));
			Vector3 vector1 = Vector3::Cross(vector3_1, vector3_2);

			Matrix lookAt;
			lookAt.M11 = vector3_2.X;
			lookAt.M12 = vector1.X;
			lookAt.M13 = vector3_1.X;
			lookAt.M14 = 0.0f;
			lookAt.M21 = vector3_2.Y;
			lookAt.M22 = vector1.Y;
			lookAt.M23 = vector3_1.Y;
			lookAt.M24 = 0.0f;
			lookAt.M31 = vector3_2.Z;
			lookAt.M32 = vector1.Z;
			lookAt.M33 = vector3_1.Z;
			lookAt.M34 = 0.0f;
			lookAt.M41 = -Vector3::Dot(vector3_2, cameraPosition);
			lookAt.M42 = -Vector3::Dot(vector1, cameraPosition);
			lookAt.M43 = -Vector3::Dot(vector3_1, cameraPosition);
			lookAt.M44 = 1.0;
			return lookAt;
		}

		bool constexpr Equals(Matrix const& other) const {
			return M11 == other.M11
				&& M22 == other.M22
				&& M33 == other.M33
				&& M44 == other.M44
				&& M12 == other.M12
				&& M13 == other.M13
				&& M14 == other.M14
				&& M21 == other.M21
				&& M23 == other.M23
				&& M24 == other.M24
				&& M31 == other.M31
				&& M32 == other.M32
				&& M34 == other.M34
				&& M41 == other.M41
				&& M42 == other.M42
				&& M43 == other.M43;
		}

		static constexpr Matrix Transpose(Matrix const& matrix)
		{
			Matrix matrix1;
			matrix1.M11 = matrix.M11;
			matrix1.M12 = matrix.M21;
			matrix1.M13 = matrix.M31;
			matrix1.M14 = matrix.M41;
			matrix1.M21 = matrix.M12;
			matrix1.M22 = matrix.M22;
			matrix1.M23 = matrix.M32;
			matrix1.M24 = matrix.M42;
			matrix1.M31 = matrix.M13;
			matrix1.M32 = matrix.M23;
			matrix1.M33 = matrix.M33;
			matrix1.M34 = matrix.M43;
			matrix1.M41 = matrix.M14;
			matrix1.M42 = matrix.M24;
			matrix1.M43 = matrix.M34;
			matrix1.M44 = matrix.M44;
			return matrix1;
		}

		double Determinant() {
			const auto m11 = M11;
			const auto m12 = M12;
			const auto m13 = M13;
			const auto m14 = M14;
			const auto m21 = M21;
			const auto m22 = M22;
			const auto m23 = M23;
			const auto m24 = M24;
			const auto m31 = M31;
			const auto m32 = M32;
			const auto m33 = M33;
			const auto m34 = M34;
			const auto m41 = M41;
			const auto m42 = M42;
			const auto m43 = M43;
			const auto m44 = M44;
			const auto num1 = (m33 * m44 - m34 * m43);
			const auto num2 = (m32 * m44 - m34 * m42);
			const auto num3 = (m32 * m43 - m33 * m42);
			const auto num4 = (m31 * m44 - m34 * m41);
			const auto num5 = (m31 * m43 - m33 * m41);
			const auto num6 = (m31 * m42 - m32 * m41);
			return (m11 * (m22 * num1 - m23 * num2 + m24 * num3) - m12 * (m21 * num1 - m23 * num4 + m24 * num5) + m13 * (m21 * num2 - m22 * num4 + m24 * num6) - m14 * (m21 * num3 - m22 * num5 + m23 * num6));
		}

		static constexpr Matrix Invert(Matrix const& matrix) {
			const auto m11 = matrix.M11;
			const auto m12 = matrix.M12;
			const auto m13 = matrix.M13;
			const auto m14 = matrix.M14;
			const auto m21 = matrix.M21;
			const auto m22 = matrix.M22;
			const auto m23 = matrix.M23;
			const auto m24 = matrix.M24;
			const auto m31 = matrix.M31;
			const auto m32 = matrix.M32;
			const auto m33 = matrix.M33;
			const auto m34 = matrix.M34;
			const auto m41 = matrix.M41;
			const auto m42 = matrix.M42;
			const auto m43 = matrix.M43;
			const auto m44 = matrix.M44;
			const auto num1 = (m33 * m44 - m34 * m43);
			const auto num2 = (m32 * m44 - m34 * m42);
			const auto num3 = (m32 * m43 - m33 * m42);
			const auto num4 = (m31 * m44 - m34 * m41);
			const auto num5 = (m31 * m43 - m33 * m41);
			const auto num6 = (m31 * m42 - m32 * m41);
			const auto num7 = (m22 * num1 - m23 * num2 + m24 * num3);
			const auto num8 = -(m21 * num1 - m23 * num4 + m24 * num5);
			const auto num9 = (m21 * num2 - m22 * num4 + m24 * num6);
			const auto num10 = -(m21 * num3 - m22 * num5 + m23 * num6);
			const auto num11 = (1.0 / (m11 * num7 + m12 * num8 + m13 * num9 + m14 * num10));

			Matrix matrix1;
			matrix1.M11 = num7 * num11;
			matrix1.M21 = num8 * num11;
			matrix1.M31 = num9 * num11;
			matrix1.M41 = num10 * num11;
			matrix1.M12 = -(m12 * num1 - m13 * num2 + m14 * num3) * num11;
			matrix1.M22 = (m11 * num1 - m13 * num4 + m14 * num5) * num11;
			matrix1.M32 = -(m11 * num2 - m12 * num4 + m14 * num6) * num11;
			matrix1.M42 = (m11 * num3 - m12 * num5 + m13 * num6) * num11;
			const auto num12 = (m23 * m44 - m24 * m43);
			const auto num13 = (m22 * m44 - m24 * m42);
			const auto num14 = (m22 * m43 - m23 * m42);
			const auto num15 = (m21 * m44 - m24 * m41);
			const auto num16 = (m21 * m43 - m23 * m41);
			const auto num17 = (m21 * m42 - m22 * m41);
			matrix1.M13 = (m12 * num12 - m13 * num13 + m14 * num14) * num11;
			matrix1.M23 = -(m11 * num12 - m13 * num15 + m14 * num16) * num11;
			matrix1.M33 = (m11 * num13 - m12 * num15 + m14 * num17) * num11;
			matrix1.M43 = -(m11 * num14 - m12 * num16 + m13 * num17) * num11;
			const auto num18 = (m23 * m34 - m24 * m33);
			const auto num19 = (m22 * m34 - m24 * m32);
			const auto num20 = (m22 * m33 - m23 * m32);
			const auto num21 = (m21 * m34 - m24 * m31);
			const auto num22 = (m21 * m33 - m23 * m31);
			const auto num23 = (m21 * m32 - m22 * m31);
			matrix1.M14 = -(m12 * num18 - m13 * num19 + m14 * num20) * num11;
			matrix1.M24 = (m11 * num18 - m13 * num21 + m14 * num22) * num11;
			matrix1.M34 = -(m11 * num19 - m12 * num21 + m14 * num23) * num11;
			matrix1.M44 = (m11 * num20 - m12 * num22 + m13 * num23) * num11;
			return matrix1;
		}

		static constexpr Matrix Lerp(Matrix const& matrix1, Matrix const& matrix2, double amount) {
			Matrix matrix;
			matrix.M11 = matrix1.M11 + (matrix2.M11 - matrix1.M11) * amount;
			matrix.M12 = matrix1.M12 + (matrix2.M12 - matrix1.M12) * amount;
			matrix.M13 = matrix1.M13 + (matrix2.M13 - matrix1.M13) * amount;
			matrix.M14 = matrix1.M14 + (matrix2.M14 - matrix1.M14) * amount;
			matrix.M21 = matrix1.M21 + (matrix2.M21 - matrix1.M21) * amount;
			matrix.M22 = matrix1.M22 + (matrix2.M22 - matrix1.M22) * amount;
			matrix.M23 = matrix1.M23 + (matrix2.M23 - matrix1.M23) * amount;
			matrix.M24 = matrix1.M24 + (matrix2.M24 - matrix1.M24) * amount;
			matrix.M31 = matrix1.M31 + (matrix2.M31 - matrix1.M31) * amount;
			matrix.M32 = matrix1.M32 + (matrix2.M32 - matrix1.M32) * amount;
			matrix.M33 = matrix1.M33 + (matrix2.M33 - matrix1.M33) * amount;
			matrix.M34 = matrix1.M34 + (matrix2.M34 - matrix1.M34) * amount;
			matrix.M41 = matrix1.M41 + (matrix2.M41 - matrix1.M41) * amount;
			matrix.M42 = matrix1.M42 + (matrix2.M42 - matrix1.M42) * amount;
			matrix.M43 = matrix1.M43 + (matrix2.M43 - matrix1.M43) * amount;
			matrix.M44 = matrix1.M44 + (matrix2.M44 - matrix1.M44) * amount;
			return matrix;
		}

		static constexpr Matrix Negate(Matrix const& matrix) {
			Matrix matrix1;
			matrix1.M11 = -matrix.M11;
			matrix1.M12 = -matrix.M12;
			matrix1.M13 = -matrix.M13;
			matrix1.M14 = -matrix.M14;
			matrix1.M21 = -matrix.M21;
			matrix1.M22 = -matrix.M22;
			matrix1.M23 = -matrix.M23;
			matrix1.M24 = -matrix.M24;
			matrix1.M31 = -matrix.M31;
			matrix1.M32 = -matrix.M32;
			matrix1.M33 = -matrix.M33;
			matrix1.M34 = -matrix.M34;
			matrix1.M41 = -matrix.M41;
			matrix1.M42 = -matrix.M42;
			matrix1.M43 = -matrix.M43;
			matrix1.M44 = -matrix.M44;
			return matrix1;
		}

		static constexpr Matrix Add(Matrix const& matrix1, Matrix const& matrix2) {
			Matrix matrix;
			matrix.M11 = matrix1.M11 + matrix2.M11;
			matrix.M12 = matrix1.M12 + matrix2.M12;
			matrix.M13 = matrix1.M13 + matrix2.M13;
			matrix.M14 = matrix1.M14 + matrix2.M14;
			matrix.M21 = matrix1.M21 + matrix2.M21;
			matrix.M22 = matrix1.M22 + matrix2.M22;
			matrix.M23 = matrix1.M23 + matrix2.M23;
			matrix.M24 = matrix1.M24 + matrix2.M24;
			matrix.M31 = matrix1.M31 + matrix2.M31;
			matrix.M32 = matrix1.M32 + matrix2.M32;
			matrix.M33 = matrix1.M33 + matrix2.M33;
			matrix.M34 = matrix1.M34 + matrix2.M34;
			matrix.M41 = matrix1.M41 + matrix2.M41;
			matrix.M42 = matrix1.M42 + matrix2.M42;
			matrix.M43 = matrix1.M43 + matrix2.M43;
			matrix.M44 = matrix1.M44 + matrix2.M44;
			return matrix;
		}

		static constexpr Matrix Subtract(Matrix const& matrix1, Matrix const& matrix2) {
			Matrix matrix;
			matrix.M11 = matrix1.M11 - matrix2.M11;
			matrix.M12 = matrix1.M12 - matrix2.M12;
			matrix.M13 = matrix1.M13 - matrix2.M13;
			matrix.M14 = matrix1.M14 - matrix2.M14;
			matrix.M21 = matrix1.M21 - matrix2.M21;
			matrix.M22 = matrix1.M22 - matrix2.M22;
			matrix.M23 = matrix1.M23 - matrix2.M23;
			matrix.M24 = matrix1.M24 - matrix2.M24;
			matrix.M31 = matrix1.M31 - matrix2.M31;
			matrix.M32 = matrix1.M32 - matrix2.M32;
			matrix.M33 = matrix1.M33 - matrix2.M33;
			matrix.M34 = matrix1.M34 - matrix2.M34;
			matrix.M41 = matrix1.M41 - matrix2.M41;
			matrix.M42 = matrix1.M42 - matrix2.M42;
			matrix.M43 = matrix1.M43 - matrix2.M43;
			matrix.M44 = matrix1.M44 - matrix2.M44;
			return matrix;
		}

		static constexpr Matrix Multiply(Matrix const& matrix1, Matrix const& matrix2) {
			Matrix matrix;
			matrix.M11 = (matrix1.M11 * matrix2.M11 + matrix1.M12 * matrix2.M21 + matrix1.M13 * matrix2.M31 + matrix1.M14 * matrix2.M41);
			matrix.M12 = (matrix1.M11 * matrix2.M12 + matrix1.M12 * matrix2.M22 + matrix1.M13 * matrix2.M32 + matrix1.M14 * matrix2.M42);
			matrix.M13 = (matrix1.M11 * matrix2.M13 + matrix1.M12 * matrix2.M23 + matrix1.M13 * matrix2.M33 + matrix1.M14 * matrix2.M43);
			matrix.M14 = (matrix1.M11 * matrix2.M14 + matrix1.M12 * matrix2.M24 + matrix1.M13 * matrix2.M34 + matrix1.M14 * matrix2.M44);
			matrix.M21 = (matrix1.M21 * matrix2.M11 + matrix1.M22 * matrix2.M21 + matrix1.M23 * matrix2.M31 + matrix1.M24 * matrix2.M41);
			matrix.M22 = (matrix1.M21 * matrix2.M12 + matrix1.M22 * matrix2.M22 + matrix1.M23 * matrix2.M32 + matrix1.M24 * matrix2.M42);
			matrix.M23 = (matrix1.M21 * matrix2.M13 + matrix1.M22 * matrix2.M23 + matrix1.M23 * matrix2.M33 + matrix1.M24 * matrix2.M43);
			matrix.M24 = (matrix1.M21 * matrix2.M14 + matrix1.M22 * matrix2.M24 + matrix1.M23 * matrix2.M34 + matrix1.M24 * matrix2.M44);
			matrix.M31 = (matrix1.M31 * matrix2.M11 + matrix1.M32 * matrix2.M21 + matrix1.M33 * matrix2.M31 + matrix1.M34 * matrix2.M41);
			matrix.M32 = (matrix1.M31 * matrix2.M12 + matrix1.M32 * matrix2.M22 + matrix1.M33 * matrix2.M32 + matrix1.M34 * matrix2.M42);
			matrix.M33 = (matrix1.M31 * matrix2.M13 + matrix1.M32 * matrix2.M23 + matrix1.M33 * matrix2.M33 + matrix1.M34 * matrix2.M43);
			matrix.M34 = (matrix1.M31 * matrix2.M14 + matrix1.M32 * matrix2.M24 + matrix1.M33 * matrix2.M34 + matrix1.M34 * matrix2.M44);
			matrix.M41 = (matrix1.M41 * matrix2.M11 + matrix1.M42 * matrix2.M21 + matrix1.M43 * matrix2.M31 + matrix1.M44 * matrix2.M41);
			matrix.M42 = (matrix1.M41 * matrix2.M12 + matrix1.M42 * matrix2.M22 + matrix1.M43 * matrix2.M32 + matrix1.M44 * matrix2.M42);
			matrix.M43 = (matrix1.M41 * matrix2.M13 + matrix1.M42 * matrix2.M23 + matrix1.M43 * matrix2.M33 + matrix1.M44 * matrix2.M43);
			matrix.M44 = (matrix1.M41 * matrix2.M14 + matrix1.M42 * matrix2.M24 + matrix1.M43 * matrix2.M34 + matrix1.M44 * matrix2.M44);
			return matrix;
		}

		static constexpr Matrix Multiply(Matrix const& matrix1, double scaleFactor) {
			const auto num = scaleFactor;
			Matrix matrix;
			matrix.M11 = matrix1.M11 * num;
			matrix.M12 = matrix1.M12 * num;
			matrix.M13 = matrix1.M13 * num;
			matrix.M14 = matrix1.M14 * num;
			matrix.M21 = matrix1.M21 * num;
			matrix.M22 = matrix1.M22 * num;
			matrix.M23 = matrix1.M23 * num;
			matrix.M24 = matrix1.M24 * num;
			matrix.M31 = matrix1.M31 * num;
			matrix.M32 = matrix1.M32 * num;
			matrix.M33 = matrix1.M33 * num;
			matrix.M34 = matrix1.M34 * num;
			matrix.M41 = matrix1.M41 * num;
			matrix.M42 = matrix1.M42 * num;
			matrix.M43 = matrix1.M43 * num;
			matrix.M44 = matrix1.M44 * num;
			return matrix;
		}

		static constexpr Matrix Divide(Matrix const& matrix1, Matrix const& matrix2) {
			Matrix matrix;
			matrix.M11 = matrix1.M11 / matrix2.M11;
			matrix.M12 = matrix1.M12 / matrix2.M12;
			matrix.M13 = matrix1.M13 / matrix2.M13;
			matrix.M14 = matrix1.M14 / matrix2.M14;
			matrix.M21 = matrix1.M21 / matrix2.M21;
			matrix.M22 = matrix1.M22 / matrix2.M22;
			matrix.M23 = matrix1.M23 / matrix2.M23;
			matrix.M24 = matrix1.M24 / matrix2.M24;
			matrix.M31 = matrix1.M31 / matrix2.M31;
			matrix.M32 = matrix1.M32 / matrix2.M32;
			matrix.M33 = matrix1.M33 / matrix2.M33;
			matrix.M34 = matrix1.M34 / matrix2.M34;
			matrix.M41 = matrix1.M41 / matrix2.M41;
			matrix.M42 = matrix1.M42 / matrix2.M42;
			matrix.M43 = matrix1.M43 / matrix2.M43;
			matrix.M44 = matrix1.M44 / matrix2.M44;
			return matrix;
		}

		static constexpr Matrix Divide(Matrix const& matrix1, double divider) {
			double num = 1.0 / divider;
			Matrix matrix;
			matrix.M11 = matrix1.M11 * num;
			matrix.M12 = matrix1.M12 * num;
			matrix.M13 = matrix1.M13 * num;
			matrix.M14 = matrix1.M14 * num;
			matrix.M21 = matrix1.M21 * num;
			matrix.M22 = matrix1.M22 * num;
			matrix.M23 = matrix1.M23 * num;
			matrix.M24 = matrix1.M24 * num;
			matrix.M31 = matrix1.M31 * num;
			matrix.M32 = matrix1.M32 * num;
			matrix.M33 = matrix1.M33 * num;
			matrix.M34 = matrix1.M34 * num;
			matrix.M41 = matrix1.M41 * num;
			matrix.M42 = matrix1.M42 * num;
			matrix.M43 = matrix1.M43 * num;
			matrix.M44 = matrix1.M44 * num;
			return matrix;
		}

		constexpr Matrix operator-() {
			return Matrix::Negate(*this);
		}

		friend constexpr bool operator ==(Matrix const& matrix1, Matrix const& matrix2) {
			return matrix1.Equals(matrix2);
		}

		friend constexpr bool operator !=(Matrix const& matrix1, Matrix const& matrix2) {
			return !matrix1.Equals(matrix2);
		}

		friend constexpr Matrix operator +(Matrix const& matrix1, Matrix const& matrix2) {
			return Matrix::Add(matrix1, matrix2);
		}

		friend constexpr Matrix operator -(Matrix const& matrix1, Matrix const& matrix2) {
			return Matrix::Subtract(matrix1, matrix2);
		}

		friend constexpr Matrix operator *(Matrix const& matrix1, Matrix const& matrix2) {
			return Matrix::Multiply(matrix1, matrix2);
		}

		friend constexpr Matrix operator *(Matrix const& matrix1, double scaleFactor) {
			return Matrix::Multiply(matrix1, scaleFactor);
		}

		friend constexpr Matrix operator *(double scaleFactor, Matrix const& matrix1) {
			return Matrix::Multiply(matrix1, scaleFactor);
		}

		friend constexpr Matrix operator /(Matrix const& matrix1, Matrix const& matrix2) {
			return Matrix::Divide(matrix1, matrix2);
		}

		friend constexpr Matrix operator /(Matrix const& matrix1, double divider) {
			return Matrix::Divide(matrix1, divider);
		}

		//implementado em quaternion.hpp

		static constexpr Matrix CreateFromQuaternion(Quaternion const& quaternion);
		static Matrix CreateFromYawPitchRoll(double yaw, double pitch, double roll);
		static constexpr Matrix Transform(Matrix const& value, Quaternion const& rotation);
		bool Decompose(Vector3& scale, Quaternion& rotation, Vector3& translation);

		static Matrix CreateShadow(Vector3 const& lightDirection, Plane const& plane);
		static Matrix CreateReflection(Plane const& value);
	};
}

//Quaternion
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

namespace xna {
	constexpr Vector2 Vector2::Transform(Vector2 const& position, Matrix const& matrix) {
		const auto num1 = (position.X * matrix.M11 + position.Y * matrix.M21) + matrix.M41;
		const auto num2 = (position.X * matrix.M12 + position.Y * matrix.M22) + matrix.M42;

		return Vector2(num1, num2);
	}

	constexpr Vector2 Vector2::TransformNormal(Vector2 const& normal, Matrix const& matrix) {
		const auto num1 = normal.X * matrix.M11 + normal.Y * matrix.M21;
		const auto num2 = normal.X * matrix.M12 + normal.Y * matrix.M22;
		return Vector2(num1, num2);
	}
}

namespace xna {
	constexpr Vector3 Vector3::Transform(Vector3 const& position, Matrix const& matrix) {
		const auto num1 = (position.X * matrix.M11 + position.Y * matrix.M21 + position.Z * matrix.M31) + matrix.M41;
		const auto num2 = (position.X * matrix.M12 + position.Y * matrix.M22 + position.Z * matrix.M32) + matrix.M42;
		const auto num3 = (position.X * matrix.M13 + position.Y * matrix.M23 + position.Z * matrix.M33) + matrix.M43;

		Vector3 vector3;
		vector3.X = num1;
		vector3.Y = num2;
		vector3.Z = num3;

		return vector3;
	}

	constexpr Vector3 Vector3::TransformNormal(Vector3 const& normal, Matrix const& matrix) {
		const auto num1 = (normal.X * matrix.M11 + normal.Y * matrix.M21 + normal.Z * matrix.M31);
		const auto num2 = (normal.X * matrix.M12 + normal.Y * matrix.M22 + normal.Z * matrix.M32);
		const auto num3 = (normal.X * matrix.M13 + normal.Y * matrix.M23 + normal.Z * matrix.M33);

		Vector3 vector3;
		vector3.X = num1;
		vector3.Y = num2;
		vector3.Z = num3;
		return vector3;
	}
}

namespace xna {
	constexpr Vector4 Vector4::Transform(Vector2 const& position, Matrix const& matrix) {
		const auto num1 = (position.X * matrix.M11 + position.Y * matrix.M21) + matrix.M41;
		const auto num2 = (position.X * matrix.M12 + position.Y * matrix.M22) + matrix.M42;
		const auto num3 = (position.X * matrix.M13 + position.Y * matrix.M23) + matrix.M43;
		const auto num4 = (position.X * matrix.M14 + position.Y * matrix.M24) + matrix.M44;
		Vector4 vector4;
		vector4.X = num1;
		vector4.Y = num2;
		vector4.Z = num3;
		vector4.W = num4;
		return vector4;
	}

	constexpr Vector4 Vector4::Transform(Vector3 const& position, Matrix const& matrix) {
		const auto num1 = (position.X * matrix.M11 + position.Y * matrix.M21 + position.Z * matrix.M31) + matrix.M41;
		const auto num2 = (position.X * matrix.M12 + position.Y * matrix.M22 + position.Z * matrix.M32) + matrix.M42;
		const auto num3 = (position.X * matrix.M13 + position.Y * matrix.M23 + position.Z * matrix.M33) + matrix.M43;
		const auto num4 = (position.X * matrix.M14 + position.Y * matrix.M24 + position.Z * matrix.M34) + matrix.M44;
		Vector4 vector4;
		vector4.X = num1;
		vector4.Y = num2;
		vector4.Z = num3;
		vector4.W = num4;
		return vector4;
	}

	constexpr Vector4 Vector4::Transform(Vector4 const& vector, Matrix const& matrix) {
		const auto num1 = (vector.X * matrix.M11 + vector.Y * matrix.M21 + vector.Z * matrix.M31 + vector.W * matrix.M41);
		const auto num2 = (vector.X * matrix.M12 + vector.Y * matrix.M22 + vector.Z * matrix.M32 + vector.W * matrix.M42);
		const auto num3 = (vector.X * matrix.M13 + vector.Y * matrix.M23 + vector.Z * matrix.M33 + vector.W * matrix.M43);
		const auto num4 = (vector.X * matrix.M14 + vector.Y * matrix.M24 + vector.Z * matrix.M34 + vector.W * matrix.M44);
		Vector4 vector4;
		vector4.X = num1;
		vector4.Y = num2;
		vector4.Z = num3;
		vector4.W = num4;
		return vector4;
	}
}

#endif