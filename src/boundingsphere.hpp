#ifndef XNA_BOUNDINGSPHERE_HPP
#define XNA_BOUNDINGSPHERE_HPP

#include <cmath>
#include "vector.hpp"
#include "mathhelper.hpp"
#include "boundingbox.hpp"
#include "matrix.hpp"

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
}

#endif