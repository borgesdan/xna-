#ifndef XNA_RECTANGLE_HPP
#define XNA_RECTANGLE_HPP

#include "point.hpp"
#include "csharp/integralnumeric.hpp"

namespace xna {
	struct Rectangle {
		csint X{ 0 };
		csint Y{ 0 };
		csint Width{ 0 };
		csint Height{ 0 };		

		constexpr Rectangle() = default;

		constexpr Rectangle(csint x, csint y, csint width, csint height):
			X(x), Y(y), Width(width), Height(height){}

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

		constexpr void Offset(Point const& amount)	{
			X += amount.X;
			Y += amount.Y;
		}

		constexpr void Offset(csint offsetX, csint offsetY)	{
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

			if (num7 > num5 && num8 > num6)	{
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

#endif