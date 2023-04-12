#ifndef XNA_RAY_HPP
#define XNA_RAY_HPP

#include <cmath>
#include <limits>
#include "vector.hpp"
#include "csharp/nullable.hpp"
#include "boundingbox.hpp"
#include "boundingsphere.hpp"
#include "plane.hpp"
#include "mathhelper.hpp"

namespace xna {
	struct BoundingFrustum;

	struct Ray {
		Vector3 Position;
		Vector3 Direction;

		constexpr Ray() = default;

		constexpr Ray(Vector3 const& position, Vector3 const& direction):
			Position(position), Direction(direction){}

		constexpr bool Equals(Ray const& other) const {
			return Position == other.Position && Direction == other.Direction;
		}

		constexpr cs::n_double Intersects(BoundingBox const& box) const {
			return box.Intersects(*this);
		}

		constexpr cs::n_double Intersects(BoundingSphere const& sphere) const {
			const auto num1 = sphere.Center.X - Position.X;
			const auto num2 = sphere.Center.Y - Position.Y;
			const auto num3 = sphere.Center.Z - Position.Z;
			const auto num4 = (num1 * num1 + num2 * num2 + num3 * num3);
			const auto num5 = sphere.Radius * sphere.Radius;

			if (num4 <= num5)
				return 0.0;

			const auto num6 = (num1 * Direction.X + num2 * Direction.Y + num3 * Direction.Z);

			if (num6 < 0.0)
				return nullptr;

			const auto num7 = num4 - num6 * num6;

			if (num7 > num5)
				return nullptr;

			const auto num8 = Math::Sqrt(num5 - num7);

			return num6 - num8;
		}
		
		constexpr cs::n_double Intersects(Plane const& plane) const {
			const auto num1 = (plane.Normal.X * Direction.X + plane.Normal.Y * Direction.Y + plane.Normal.Z * Direction.Z);

			if (Math::Abs(num1) < 9.9999997473787516E-06)
				return nullptr;

			const auto num2 = (plane.Normal.X * Position.X + plane.Normal.Y * Position.Y + plane.Normal.Z * Position.Z);
			auto num3 = (-plane.D - num2) / num1;

			if (num3 < 0.0) {
				if (num3 < -9.9999997473787516E-06)
					return nullptr;

				num3 = 0.0;
			}
			return cs::n_double(num3);
		}

		friend constexpr bool operator==(Ray const& a, Ray const& b) {
			return a.Equals(b);
		}

		friend constexpr bool operator!=(Ray const& a, Ray const& b) {
			return !a.Equals(b);
		}

		constexpr cs::n_double Intersects(BoundingFrustum& frustum);
	};
}

namespace xna {
    constexpr cs::n_double BoundingBox::Intersects(Ray const& ray) const {
        auto num1 = 0.0;
        auto num2 = std::numeric_limits<double>::max();

        if (Math::Abs(ray.Direction.X) < 9.9999999747524271E-07) {
            if (ray.Position.X < Min.X || ray.Position.X > Max.X)
                return nullptr;
        }
        else {
            const auto num3 = 1.0 / ray.Direction.X;
            auto num4 = (Min.X - ray.Position.X) * num3;
            auto num5 = (Max.X - ray.Position.X) * num3;

            if (num4 > num5) {
                auto num6 = num4;
                num4 = num5;
                num5 = num6;
            }

            num1 = Math::Max(num4, num1);
            num2 = Math::Min(num5, num2);

            if (num1 > num2)
                return nullptr;
        }

        if (Math::Abs(ray.Direction.Y) < 9.9999999747524271E-07) {
            if (ray.Position.Y < Min.Y || ray.Position.Y > Max.Y)
                return nullptr;
        }
        else {
            const auto num7 = 1.0 / ray.Direction.Y;
            auto num8 = (Min.Y - ray.Position.Y) * num7;
            auto num9 = (Max.Y - ray.Position.Y) * num7;

            if (num8 > num9) {
                auto num10 = num8;
                num8 = num9;
                num9 = num10;
            }

            num1 = Math::Max(num8, num1);
            num2 = Math::Min(num9, num2);

            if (num1 > num2)
                return nullptr;
        }

        if (Math::Abs(ray.Direction.Z) < 9.9999999747524271E-07) {
            if (ray.Position.Z < Min.Z || ray.Position.Z > Max.Z)
                return nullptr;
        }
        else {
            const auto num11 = 1.0 / ray.Direction.Z;
            auto num12 = (Min.Z - ray.Position.Z) * num11;
            auto num13 = (Max.Z - ray.Position.Z) * num11;

            if (num12 > num13) {
                auto num14 = num12;
                num12 = num13;
                num13 = num14;
            }

            num1 = Math::Max(num12, num1);
            auto num15 = Math::Min(num13, num2);

            if (num1 > num15)
                return nullptr;
        }
        return cs::n_double(num1);
    }

    constexpr cs::n_double BoundingSphere::Intersects(Ray const& ray) const {
        return ray.Intersects(*this);
    }
}

#endif