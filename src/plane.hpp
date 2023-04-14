#ifndef XNA_PLANE_HPP
#define XNA_PLANE_HPP

#include "basic-structs.hpp"
#include "enumerations.hpp"
#include "mathhelper.hpp"
#include "matrix.hpp"
#include "quaternion.hpp"
#include "boundingbox.hpp"
#include "boundingsphere.hpp"
#include "enumerations.hpp"

namespace xna {
	struct Plane {
		Vector3 Normal{ Vector3::Zero() };
		double D{ 0.0 };

		constexpr Plane() = default;

		constexpr Plane(Vector4 const& value) :
			Normal(Vector3(value.X, value.Y, value.Z)), D(value.W){}

		constexpr Plane(Vector3 const& normal, double d) :
			Normal(normal), D(d){}

		constexpr Plane(Vector3 const& a, Vector3 const& b, Vector3 const& c):
			Normal(Vector3::Normalize(Vector3::Cross(b-a, c-a))),
			D(-(Vector3::Dot(Normal, a))) {}

		constexpr Plane(double a, double b, double c, double d):
			Normal(Vector3(a, b, c)), D(d){}

		constexpr Plane(Vector3 const& pointOnPlane, Vector3 const& normal) :
			Normal(normal), D(-(pointOnPlane.X * normal.X + pointOnPlane.Y * normal.Y + pointOnPlane.Z * normal.Z)) {
		}

		constexpr PlaneIntersectionType Intersects(Vector3 const& point) const {
			return PlaneIntersectionType::Back;
		}

		constexpr bool Equals(Plane const& other) const {
			return Normal == other.Normal
				&& D == other.D;
		}

		void Normalize();
		static Plane Normalize(Plane const& value);

		static constexpr Plane Transform(Plane const& plane, Matrix const& matrix) {
			Matrix result = Matrix::Invert(matrix);
			const auto x = plane.Normal.X;
			const auto y = plane.Normal.Y;
			const auto z = plane.Normal.Z;
			const auto d = plane.D;
			
			Plane plane1;
			plane1.Normal.X = (x * result.M11 + y * result.M12 + z * result.M13 + d * result.M14);
			plane1.Normal.Y = (x * result.M21 + y * result.M22 + z * result.M23 + d * result.M24);
			plane1.Normal.Z = (x * result.M31 + y * result.M32 + z * result.M33 + d * result.M34);
			plane1.D = (x * result.M41 + y * result.M42 + z * result.M43 + d * result.M44);
			return plane1;
		}

		static constexpr Plane Transform(Plane const& plane, Quaternion const& rotation) {
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
			const auto x = plane.Normal.X;
			const auto y = plane.Normal.Y;
			const auto z = plane.Normal.Z;
			
			Plane plane1;
			plane1.Normal.X = (x * num13 + y * num14 + z * num15);
			plane1.Normal.Y = (x * num16 + y * num17 + z * num18);
			plane1.Normal.Z = (x * num19 + y * num20 + z * num21);
			plane1.D = plane.D;
			
			return plane1;
		}

		constexpr double Dot(Vector4 const& value) const {
			return (Normal.X * value.X + Normal.Y * value.Y + Normal.Z * value.Z + D * value.W);
		}

		constexpr double DotCoordinate(Vector3 const& value) const {
			return (Normal.X * value.X + Normal.Y * value.Y + Normal.Z * value.Z) + D;
		}

		constexpr double DotNormal(Vector3 const& value) const {
			return (Normal.X * value.X + Normal.Y * value.Y + Normal.Z * value.Z);
		}

		constexpr PlaneIntersectionType Intersects(BoundingBox const& box) const {
			Vector3 vector3_1;
			vector3_1.X = Normal.X >= 0.0 ? box.Min.X : box.Max.X;
			vector3_1.Y = Normal.Y >= 0.0 ? box.Min.Y : box.Max.Y;
			vector3_1.Z = Normal.Z >= 0.0 ? box.Min.Z : box.Max.Z;

			Vector3 vector3_2;
			vector3_2.X = Normal.X >= 0.0 ? box.Max.X : box.Min.X;
			vector3_2.Y = Normal.Y >= 0.0 ? box.Max.Y : box.Min.Y;
			vector3_2.Z = Normal.Z >= 0.0 ? box.Max.Z : box.Min.Z;

			if (Normal.X * vector3_1.X + Normal.Y * vector3_1.Y + Normal.Z * vector3_1.Z + D > 0.0)
				return PlaneIntersectionType::Front;

			return Normal.X * vector3_2.X + Normal.Y * vector3_2.Y + Normal.Z * vector3_2.Z + D < 0.0 ? PlaneIntersectionType::Back : PlaneIntersectionType::Intersecting;
		}

		constexpr PlaneIntersectionType Intersects(BoundingSphere const& sphere) const {
			const auto num = (sphere.Center.X * Normal.X + sphere.Center.Y * Normal.Y + sphere.Center.Z * Normal.Z) + D;
			
			if (num >  sphere.Radius)
				return PlaneIntersectionType::Front;

			return num < -sphere.Radius ? PlaneIntersectionType::Back : PlaneIntersectionType::Intersecting;
		}

		constexpr friend bool operator==(Plane const& a, Plane const& b) {
			return a.Equals(b);
		}

		constexpr friend bool operator!=(Plane const& a, Plane const& b) {
			return !a.Equals(b);
		}

		//Implementado em frustum.hpp
		constexpr PlaneIntersectionType Intersects(BoundingFrustum const& frustum) const;
	};

	struct PlaneHelper {
		static constexpr double ClassifyPoint(Vector3 const& point, Plane const& plane) {
			return point.X * plane.Normal.X + point.Y * plane.Normal.Y + point.Z * plane.Normal.Z + plane.D;
		}
	};
}

#endif