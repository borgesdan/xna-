#ifndef XNA_COLLISION_HPP
#define XNA_COLLISION_HPP

#include <vector>
#include <cmath>
#include <limits>
#include "csharp/integralnumeric.hpp"
#include "csharp/nullable.hpp"
#include "basic-structs.hpp"
#include "enumerations.hpp"
#include "matrix.hpp"
#include "quaternion.hpp"
#include "csharp/nullable.hpp"

//BoudingBox
namespace xna {
	struct BoundingSphere;
	struct BoundingFrustum;
	struct Plane;
	struct Ray;

	struct BoundingBox {
		static constexpr csint CORNER_COUNT = 8;

		Vector3 Min{ Vector3::Zero() };
		Vector3 Max{ Vector3::Zero() };

		constexpr BoundingBox() = default;

		constexpr BoundingBox(Vector3 const& min, Vector3 const& max) :
			Min(min), Max(max) {}

		constexpr std::vector<Vector3> GetCorners() const {
			return std::vector<Vector3>{
				Vector3(Min.X, Max.Y, Max.Z),
					Vector3(Max.X, Max.Y, Max.Z),
					Vector3(Max.X, Min.Y, Max.Z),
					Vector3(Min.X, Min.Y, Max.Z),
					Vector3(Min.X, Max.Y, Min.Z),
					Vector3(Max.X, Max.Y, Min.Z),
					Vector3(Max.X, Min.Y, Min.Z),
					Vector3(Min.X, Min.Y, Min.Z)
			};
		}

		constexpr void GetCorners(std::vector<Vector3>& corners) const {
			if (corners.size() < 8)
				corners.resize(8);

			corners[0].X = Min.X;
			corners[0].Y = Max.Y;
			corners[0].Z = Max.Z;
			corners[1].X = Max.X;
			corners[1].Y = Max.Y;
			corners[1].Z = Max.Z;
			corners[2].X = Max.X;
			corners[2].Y = Min.Y;
			corners[2].Z = Max.Z;
			corners[3].X = Min.X;
			corners[3].Y = Min.Y;
			corners[3].Z = Max.Z;
			corners[4].X = Min.X;
			corners[4].Y = Max.Y;
			corners[4].Z = Min.Z;
			corners[5].X = Max.X;
			corners[5].Y = Max.Y;
			corners[5].Z = Min.Z;
			corners[6].X = Max.X;
			corners[6].Y = Min.Y;
			corners[6].Z = Min.Z;
			corners[7].X = Min.X;
			corners[7].Y = Min.Y;
			corners[7].Z = Min.Z;
		}

		constexpr bool Equals(BoundingBox const& other) const {
			return Min == other.Min && Max == other.Max;
		}

		static constexpr BoundingBox CreateMerged(BoundingBox const& original, BoundingBox const& additional) {
			BoundingBox merged;
			merged.Min = Vector3::Min(original.Min, additional.Min);
			merged.Max = Vector3::Max(original.Max, additional.Max);
			return merged;
		}

		static constexpr BoundingBox CreateFromPoints(std::vector<Vector3>& points) {
			bool flag = false;
			Vector3 result1 = Vector3(std::numeric_limits<double>::max());
			Vector3 result2 = Vector3(std::numeric_limits<double>::min());

			auto size = points.size();

			for (size_t i = 0; i < size; ++i) {
				auto& point = points[i];
				result1 = Vector3::Min(result1, point);
				result2 = Vector3::Max(result2, point);
				flag = true;
			}

			if (!flag)
				BoundingBox();

			return BoundingBox(result1, result2);
		}

		constexpr bool Intersects(BoundingBox const& box) const {
			return Max.X >= box.Min.X
				&& Min.X <= box.Max.X
				&& Max.Y >= box.Min.Y
				&& Min.Y <= box.Max.Y
				&& Max.Z >= box.Min.Z
				&& Min.Z <= box.Max.Z;
		}

		constexpr ContainmentType Contains(BoundingBox const& box) const {
			if (Max.X < box.Min.X || Min.X > box.Max.X || Max.Y < box.Min.Y || Min.Y > box.Max.Y || Max.Z < box.Min.Z || Min.Z > box.Max.Z)
				return ContainmentType::Disjoint;

			return Min.X > box.Min.X || box.Max.X > Max.X || Min.Y > box.Min.Y || box.Max.Y > Max.Y || Min.Z > box.Min.Z || box.Max.Z > Max.Z
				? ContainmentType::Intersects
				: ContainmentType::Contains;
		}

		constexpr ContainmentType Contains(Vector3 const& point) const {
			return Min.X > point.X || point.X > Max.X || Min.Y > point.Y || point.Y > Max.Y || Min.Z > point.Z || point.Z > Max.Z ? ContainmentType::Disjoint : ContainmentType::Contains;
		}

		constexpr Vector3 SupportMapping(Vector3 const& v) const {
			Vector3 result;
			result.X = v.X >= 0.0 ? Max.X : Min.X;
			result.Y = v.Y >= 0.0 ? Max.Y : Min.Y;
			result.Z = v.Z >= 0.0 ? Max.Z : Min.Z;

			return result;
		}

		constexpr friend bool operator==(BoundingBox const& a, BoundingBox const& b) {
			return a.Equals(b);
		}

		constexpr friend bool operator!=(BoundingBox const& a, BoundingBox const& b) {
			return !a.Equals(b);
		}

		//Implementado em BoundingFrustum

		constexpr bool Intersects(BoundingFrustum& frustum) const;
		constexpr ContainmentType Contains(BoundingFrustum& frustum) const;

		//Implementado em BoundingSphere

		static constexpr BoundingBox CreateFromSphere(BoundingSphere const& sphere);
		constexpr bool Intersects(BoundingSphere const& sphere) const;
		constexpr ContainmentType Contains(BoundingSphere const& sphere) const;

		//Implementado em Plane e Ray

		constexpr PlaneIntersectionType Intersects(Plane const& plane) const;
		constexpr cs::n_double Intersects(Ray const& ray) const;

	};
}

//BoundingSphere
namespace xna {
	struct BoundingSphere {
		Vector3 Center{ Vector3::Zero() };
		double Radius{ 0.0 };

		constexpr BoundingSphere() = default;

		constexpr BoundingSphere(Vector3 center, double radius) :
			Center(center), Radius(radius >= 0.0 ? radius : 0.0) {}

		constexpr bool Equals(BoundingSphere const& other) const {
			return Center == other.Center && Radius == other.Radius;
		}

		constexpr bool Intersects(BoundingBox const& box) const {
			const auto result1 = Vector3::Clamp(Center, box.Min, box.Max);
			const auto result2 = Vector3::DistanceSquared(Center, result1);

			return result2 <= Radius * Radius;
		}

		constexpr bool Intersects(BoundingSphere const& sphere) const {
			const auto result = Vector3::DistanceSquared(Center, sphere.Center);
			const auto radius1 = Radius;
			const auto radius2 = sphere.Radius;

			return radius1 * radius1 + 2.0 * radius1 * radius2 + radius2 * radius2 > result;
		}

		static BoundingSphere CreateFromBoundingBox(BoundingBox const& box);
		static BoundingSphere CreateFromPoints(std::vector<Vector3> const& points);
		static BoundingSphere CreateMerged(BoundingSphere const& original, BoundingSphere const& additional);

		constexpr ContainmentType Contains(BoundingBox const& box) const {
			if (!box.Intersects(*this))
				return ContainmentType::Disjoint;

			const auto num = Radius * Radius;

			Vector3 vector3;
			vector3.X = Center.X - box.Min.X;
			vector3.Y = Center.Y - box.Max.Y;
			vector3.Z = Center.Z - box.Max.Z;

			if (vector3.LengthSquared() > num)
				return ContainmentType::Intersects;

			vector3.X = Center.X - box.Max.X;
			vector3.Y = Center.Y - box.Max.Y;
			vector3.Z = Center.Z - box.Max.Z;

			if (vector3.LengthSquared() > num)
				return ContainmentType::Intersects;

			vector3.X = Center.X - box.Max.X;
			vector3.Y = Center.Y - box.Min.Y;
			vector3.Z = Center.Z - box.Max.Z;

			if (vector3.LengthSquared() > num)
				return ContainmentType::Intersects;

			vector3.X = Center.X - box.Min.X;
			vector3.Y = Center.Y - box.Min.Y;
			vector3.Z = Center.Z - box.Max.Z;

			if (vector3.LengthSquared() > num)
				return ContainmentType::Intersects;

			vector3.X = Center.X - box.Min.X;
			vector3.Y = Center.Y - box.Max.Y;
			vector3.Z = Center.Z - box.Min.Z;

			if (vector3.LengthSquared() > num)
				return ContainmentType::Intersects;

			vector3.X = Center.X - box.Max.X;
			vector3.Y = Center.Y - box.Max.Y;
			vector3.Z = Center.Z - box.Min.Z;

			if (vector3.LengthSquared() > num)
				return ContainmentType::Intersects;

			vector3.X = Center.X - box.Max.X;
			vector3.Y = Center.Y - box.Min.Y;
			vector3.Z = Center.Z - box.Min.Z;

			if (vector3.LengthSquared() > num)
				return ContainmentType::Intersects;

			vector3.X = Center.X - box.Min.X;
			vector3.Y = Center.Y - box.Min.Y;
			vector3.Z = Center.Z - box.Min.Z;

			return vector3.LengthSquared() > num ? ContainmentType::Intersects : ContainmentType::Contains;
		}

		constexpr ContainmentType Contains(Vector3 const& point) const {
			return Vector3::DistanceSquared(point, Center) >= Radius * Radius ? ContainmentType::Disjoint : ContainmentType::Contains;
		}

		ContainmentType Contains(BoundingSphere const& sphere) const;

		Vector3 SupportMapping(Vector3 const& v) const {
			const auto num = Radius / v.Length();

			Vector3 result;
			result.X = Center.X + v.X * num;
			result.Y = Center.Y + v.Y * num;
			result.Z = Center.Z + v.Z * num;

			return result;
		}

		BoundingSphere Transform(Matrix const& matrix) const;

		constexpr friend bool operator==(BoundingSphere const& a, BoundingSphere const& b) {
			return a.Equals(b);
		}

		constexpr friend bool operator!=(BoundingSphere const& a, BoundingSphere const& b) {
			return !a.Equals(b);
		}

		//Implementado em boundingfrustum, plane, ray

		constexpr bool Intersects(BoundingFrustum& frustum) const;
		constexpr ContainmentType Contains(BoundingFrustum const& frustum) const;

		constexpr PlaneIntersectionType Intersects(Plane const& plane) const;
		constexpr cs::Nullable<double> Intersects(Ray const& ray) const;
	};
}

//Plane
namespace xna {
	struct Plane {
		Vector3 Normal{ Vector3::Zero() };
		double D{ 0.0 };

		constexpr Plane() = default;

		constexpr Plane(Vector4 const& value) :
			Normal(Vector3(value.X, value.Y, value.Z)), D(value.W) {}

		constexpr Plane(Vector3 const& normal, double d) :
			Normal(normal), D(d) {}

		constexpr Plane(Vector3 const& a, Vector3 const& b, Vector3 const& c) :
			Normal(Vector3::Normalize(Vector3::Cross(b - a, c - a))),
			D(-(Vector3::Dot(Normal, a))) {}

		constexpr Plane(double a, double b, double c, double d) :
			Normal(Vector3(a, b, c)), D(d) {}

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

			if (num > sphere.Radius)
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

//Ray
namespace xna {
	struct BoundingFrustum;

	struct Ray {
		Vector3 Position;
		Vector3 Direction;

		constexpr Ray() = default;

		constexpr Ray(Vector3 const& position, Vector3 const& direction) :
			Position(position), Direction(direction) {}

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

//BoundingFrustum
namespace xna {
	struct BoundingFrustum {
		static constexpr csint PlaneCount{ 6 };
		static constexpr csint CornerCount{ 8 };

		constexpr BoundingFrustum(Matrix const& value) :
			_matrix(value) {
			CreatePlanes();
			CreateCorners();
		}

		constexpr Matrix Matrix_() const {
			return _matrix;
		}

		constexpr void Matrix_(Matrix const& value) {
			_matrix = value;
			CreatePlanes();
			CreateCorners();
		}

		constexpr Plane Near() const {
			return _planes[0];
		}

		constexpr Plane Far() const {
			return _planes[1];
		}

		constexpr Plane Left() const {
			return _planes[2];
		}

		constexpr Plane Right() const {
			return _planes[3];
		}

		constexpr Plane Top() const {
			return _planes[4];
		}

		constexpr Plane Bottom() const {
			return _planes[5];
		}

		constexpr ContainmentType Contains(BoundingBox const& box) const {
			auto intersects = false;

			for (size_t i = 0; i < PlaneCount; ++i) {
				auto planeIntersectionType = box.Intersects(_planes[i]);
				switch (planeIntersectionType)
				{
				case PlaneIntersectionType::Front:
					return ContainmentType::Disjoint;
				case PlaneIntersectionType::Intersecting:
					intersects = true;
					break;
				}
			}

			return intersects ? ContainmentType::Intersects : ContainmentType::Contains;
		}

		constexpr ContainmentType Contains(BoundingFrustum const& frustum) const {
			if (*this == frustum)
				return ContainmentType::Contains;

			auto intersects = false;

			for (size_t i = 0; i < PlaneCount; ++i) {
				PlaneIntersectionType planeIntersectionType = frustum.Intersects(_planes[i]);
				switch (planeIntersectionType)
				{
				case PlaneIntersectionType::Front:
					return ContainmentType::Disjoint;
				case PlaneIntersectionType::Intersecting:
					intersects = true;
					break;
				}
			}
			return intersects ? ContainmentType::Intersects : ContainmentType::Contains;
		}

		constexpr ContainmentType Contains(BoundingSphere const& sphere) const {
			auto intersects = false;
			for (size_t i = 0; i < PlaneCount; ++i) {
				auto planeIntersectionType = sphere.Intersects(_planes[i]);

				switch (planeIntersectionType)
				{
				case PlaneIntersectionType::Front:
					return ContainmentType::Disjoint;
				case PlaneIntersectionType::Intersecting:
					intersects = true;
					break;
				}
			}
			return intersects ? ContainmentType::Intersects : ContainmentType::Contains;
		}

		constexpr ContainmentType Contains(Vector3 const& point) const {
			for (size_t i = 0; i < PlaneCount; ++i) {
				if (PlaneHelper::ClassifyPoint(point, _planes[i]) > 0) {
					return ContainmentType::Disjoint;
				}
			}

			return ContainmentType::Contains;
		}

		constexpr std::vector<Vector3> GetCorners() const {
			return _corners;
		}

		constexpr void GetCorners(std::vector<Vector3>& corners) {
			if (corners.size() < 8)
				corners.resize(8);

			corners = _corners;
		}

		constexpr bool Intersects(BoundingBox const& box) const {
			return Contains(box) != ContainmentType::Disjoint;
		}

		constexpr bool Intersects(BoundingFrustum const& frustum) const {
			return Contains(frustum) != ContainmentType::Disjoint;
		}

		constexpr bool Intersects(BoundingSphere const& sphere) const {
			return Contains(sphere) != ContainmentType::Disjoint;
		}

		constexpr PlaneIntersectionType Intersects(Plane const& plane) const {
			auto result = plane.Intersects(_corners[0]);

			const auto size = _corners.size();
			for (size_t i = 1; i < size; i++)
				if (plane.Intersects(_corners[i]) != result)
					result = PlaneIntersectionType::Intersecting;

			return result;
		}

		constexpr cs::n_double Intersects(Ray const& ray) const {
			ContainmentType ctype = Contains(ray.Position);

			switch (ctype)
			{
			case ContainmentType::Disjoint:
				return nullptr;
			case ContainmentType::Contains:
				return 0.0;
			case ContainmentType::Intersects:
				return nullptr;
			default:
				return nullptr;
			}
		}

		constexpr bool Equals(BoundingFrustum const& other) const {
			return _matrix == other._matrix;
		}

		friend constexpr bool operator ==(BoundingFrustum const& a, BoundingFrustum const& b) {
			return a.Equals(b);
		}

		friend constexpr bool operator !=(BoundingFrustum const& a, BoundingFrustum const& b) {
			return !a.Equals(b);
		}

	private:
		Matrix _matrix;
		std::vector<Vector3> _corners = std::vector<Vector3>();
		std::vector<Plane> _planes = std::vector<Plane>();


		constexpr void CreateCorners() {
			IntersectionPoint(_planes[0], _planes[2], _planes[4], _corners[0]);
			IntersectionPoint(_planes[0], _planes[3], _planes[4], _corners[1]);
			IntersectionPoint(_planes[0], _planes[3], _planes[5], _corners[2]);
			IntersectionPoint(_planes[0], _planes[2], _planes[5], _corners[3]);
			IntersectionPoint(_planes[1], _planes[2], _planes[4], _corners[4]);
			IntersectionPoint(_planes[1], _planes[3], _planes[4], _corners[5]);
			IntersectionPoint(_planes[1], _planes[3], _planes[5], _corners[6]);
			IntersectionPoint(_planes[1], _planes[2], _planes[5], _corners[7]);
		}

		constexpr void CreatePlanes() {
			_planes[0] = Plane(-_matrix.M13, -_matrix.M23, -_matrix.M33, -_matrix.M43);
			_planes[1] = Plane(_matrix.M13 - _matrix.M14, _matrix.M23 - _matrix.M24, _matrix.M33 - _matrix.M34, _matrix.M43 - _matrix.M44);
			_planes[2] = Plane(-_matrix.M14 - _matrix.M11, -_matrix.M24 - _matrix.M21, -_matrix.M34 - _matrix.M31, -_matrix.M44 - _matrix.M41);
			_planes[3] = Plane(_matrix.M11 - _matrix.M14, _matrix.M21 - _matrix.M24, _matrix.M31 - _matrix.M34, _matrix.M41 - _matrix.M44);
			_planes[4] = Plane(_matrix.M12 - _matrix.M14, _matrix.M22 - _matrix.M24, _matrix.M32 - _matrix.M34, _matrix.M42 - _matrix.M44);
			_planes[5] = Plane(-_matrix.M14 - _matrix.M12, -_matrix.M24 - _matrix.M22, -_matrix.M34 - _matrix.M32, -_matrix.M44 - _matrix.M42);

			NormalizePlane(_planes[0]);
			NormalizePlane(_planes[1]);
			NormalizePlane(_planes[2]);
			NormalizePlane(_planes[3]);
			NormalizePlane(_planes[4]);
			NormalizePlane(_planes[5]);
		}

		constexpr void NormalizePlane(Plane& p) {
			const auto factor = 1.0 / p.Normal.Length();
			p.Normal.X *= factor;
			p.Normal.Y *= factor;
			p.Normal.Z *= factor;
			p.D *= factor;
		}

		static constexpr void IntersectionPoint(Plane const& a, Plane const& b, Plane const& c, Vector3& result) {

			Vector3 v1, v2, v3;
			Vector3 cross = Vector3::Cross(b.Normal, c.Normal);

			auto f = Vector3::Dot(a.Normal, cross);
			f *= -1.0;

			cross = Vector3::Cross(b.Normal, c.Normal);
			v1 = Vector3::Multiply(cross, a.D);

			cross = Vector3::Cross(c.Normal, a.Normal);
			v2 = Vector3::Multiply(cross, b.D);

			cross = Vector3::Cross(a.Normal, b.Normal);
			v3 = Vector3::Multiply(cross, c.D);

			result.X = (v1.X + v2.X + v3.X) / f;
			result.Y = (v1.Y + v2.Y + v3.Y) / f;
			result.Z = (v1.Z + v2.Z + v3.Z) / f;
		}
	};
}

namespace xna {	
}

namespace xna {
	constexpr BoundingBox BoundingBox::CreateFromSphere(BoundingSphere const& sphere) {
		BoundingBox fromSphere;
		fromSphere.Min.X = sphere.Center.X - sphere.Radius;
		fromSphere.Min.Y = sphere.Center.Y - sphere.Radius;
		fromSphere.Min.Z = sphere.Center.Z - sphere.Radius;
		fromSphere.Max.X = sphere.Center.X + sphere.Radius;
		fromSphere.Max.Y = sphere.Center.Y + sphere.Radius;
		fromSphere.Max.Z = sphere.Center.Z + sphere.Radius;
		return fromSphere;
	}

	constexpr bool BoundingBox::Intersects(BoundingSphere const& sphere) const {
		const auto result1 = Vector3::Clamp(sphere.Center, Min, Max);
		const auto result2 = Vector3::DistanceSquared(sphere.Center, result1);

		return result2 <= sphere.Radius * sphere.Radius;
	}

	constexpr ContainmentType BoundingBox::Contains(BoundingSphere const& sphere) const {
		const auto result1 = Vector3::Clamp(sphere.Center, Min, Max);
		const auto result2 = Vector3::DistanceSquared(sphere.Center, result1);
		const auto radius = sphere.Radius;

		if (result2 > radius * radius)
			return ContainmentType::Disjoint;

		return Min.X + radius > sphere.Center.X
			|| sphere.Center.X > Max.X - radius
			|| Max.X - Min.X <= radius
			|| Min.Y + radius > sphere.Center.Y
			|| sphere.Center.Y > Max.Y - radius
			|| Max.Y - Min.Y <= radius
			|| Min.Z + radius > sphere.Center.Z
			|| sphere.Center.Z > Max.Z - radius
			|| Max.X - Min.X <= radius
			? ContainmentType::Intersects
			: ContainmentType::Contains;
	}

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

	constexpr bool BoundingBox::Intersects(BoundingFrustum& frustum) const {
		return frustum.Intersects(*this);
	}

	constexpr ContainmentType BoundingBox::Contains(BoundingFrustum& frustum) const {
		if (!frustum.Intersects(*this))
			return ContainmentType::Disjoint;

		const auto corners = frustum.GetCorners();
		const auto size = corners.size();

		for (size_t i = 0; i < size; ++i) {
			const auto& corner = corners[i];

			if (Contains(corner) == ContainmentType::Disjoint)
				return ContainmentType::Intersects;
		}

		return ContainmentType::Contains;
	}	
}

namespace xna {
	constexpr cs::n_double BoundingSphere::Intersects(Ray const& ray) const {
		return ray.Intersects(*this);
	}

	constexpr bool BoundingSphere::Intersects(BoundingFrustum& frustum) const {
		return frustum.Intersects(*this);
	}

	constexpr ContainmentType BoundingSphere::Contains(BoundingFrustum const& frustum) const {
		bool inside = true;

		const auto corners = frustum.GetCorners();
		const auto size = corners.size();
		for (size_t i = 0; i < size; ++i) {
			const auto& corner = corners[i];

			if (Contains(corner) == ContainmentType::Disjoint) {
				inside = false;
				break;
			}
		}
		if (inside)
			return ContainmentType::Contains;

		const auto dmin = 0.0;

		if (dmin <= Radius * Radius)
			return ContainmentType::Intersects;

		return ContainmentType::Disjoint;
	}
}

namespace xna {
	constexpr cs::n_double Ray::Intersects(BoundingFrustum& frustum) {
		return frustum.Intersects(*this);
	}
}

namespace xna {
	constexpr PlaneIntersectionType Plane::Intersects(BoundingFrustum const& frustum) const {
		return frustum.Intersects(*this);
	}
}



#endif