#ifndef XNA_POINT_HPP
#define XNA_POINT_HPP

#include "csharp/IntegralNumeric.hpp"

namespace xna {
	struct Point {
		csint X{ 0 };
		csint Y{ 0 };		

		constexpr Point() = default;
		constexpr Point(csint x, csint y) :
			X(x), Y(y){}		

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

#endif